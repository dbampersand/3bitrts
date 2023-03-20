#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "zlib.h"

#include "replay.h"
#include "gameobject.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "colors.h"
#include "map.h"

const char* REPLAY_HEADER = "REP";

 Replay replay = {0};
 UIElement replayPlayButton = {0}; 
 UIElement replayBackButton = {0}; 
 

 long bufferPosition = 0;
 char* replayBuffer = NULL;

 int soundPlayedThisFramePosition = 0;

 FILE* tempFile = NULL;


//zlib defines
#define CHUNK 16384
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif


//FORMAT:
//uint32_t number of bytes in the current frame
//RLE encoded array of bytes representing the screen state
//represented as the Color indices defined in color.h Color enum

void InitReplay()
{
    replay.frames = NULL;
    replay.numFrames = 0;
    replay.playing = true;
    replayBuffer = malloc(REPLAY_PREALLOC*sizeof(char));
    replay.frames = calloc(FRAMES_PREALLOC,sizeof(ReplayFrame));
    
    remove("replays/" TEMP_REPLAY_NAME);

}
void NewReplay()
{
    bufferPosition = 0; 
    if (!replayBuffer)
        replayBuffer = malloc(REPLAY_PREALLOC*sizeof(char));

    if (!replay.frames)
        replay.frames = malloc(FRAMES_PREALLOC*sizeof(ReplayFrame));
    replay.numFrames = 0;
    replay.framePlayPosition = 0;
    replay.playing = false;
    replay.totalFrames = 0;
}
void LoadFrame(ALLEGRO_BITMAP* screen, ReplayFrame* frame)
{
    if (!frame) return;
    int x=0; int y=0;
    /*al_lock_bitmap(screen,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);
    for (int i = 0; i < frame->dataLen; i += (sizeof(uint16_t)+sizeof(char)))
    {
        uint16_t size; 
        memcpy(&size,&frame->compressedData[i],sizeof(uint16_t));
        char c = frame->compressedData[i+sizeof(uint16_t)];

        while (size > 0)
        {
            al_put_pixel(x,y,GetColor(c,0));
            x++;
            size--;
            if (x >= _SCREEN_SIZE)
            {
                y++;
                x = 0;
            }
        }

    }
   al_unlock_bitmap(screen);*/
   al_clear_to_color(BG);

    //TODO: would be nice to just hook this up to be able to run in Render
    //instead of drawing everything manually here
    //but this works for now 
    Map m = {0};
    m.spriteIndex = frame->mapSpriteIndex;
    DrawMap(&m,false);

   for (int i = 0; i < frame->numAttacks; i++)
   {
        UpdateScreenPositionsAttack(&frame->attacks[i]);
        DrawAttack(&frame->attacks[i],0);
   }
        
   for (int i = 0; i < frame->numObjects; i++)
   {
        GameObject* g = &frame->objects[i];
        UpdateScreenPositions(g);
        g->properties |= OBJ_ACTIVE;
        DrawGameObj(g,false);
        //objects[i] = frame->objects[i];
        Sprite* s = &replay.sprites[g->spriteIndex];
        float x = g->position.screenX + g->offset.x; 
        float y = g->position.screenY + g->offset.y;

        ALLEGRO_COLOR c = GetColor(GameObjToColor(g),GetPlayerOwnedBy(g));

        Rect selectRect;
        selectRect.w = al_get_bitmap_width(s->sprite);
        selectRect.h = (al_get_bitmap_height(s->sprite));
        selectRect.x = x;
        selectRect.y = (y + (al_get_bitmap_height(s->sprite) - selectRect.h));
        if (g->stunTimer == 0)
            DrawRoundedRect(selectRect, c,false);

   }

}
void RecordReplay(ALLEGRO_BITMAP* screen)
{
    //create new frame
    ReplayFrame rf = SaveFrame(screen);
    replay.numFrames++;

    //if (replay.frames)
      //  replay.frames = realloc(replay.frames,replay.numFrames*sizeof(ReplayFrame));
    //else
    //{
   //     replay.numFrames = 1;
    //    replay.frames = calloc(replay.numFrames,sizeof(ReplayFrame));
    //}
    replay.frames[replay.numFrames-1] = rf;
   if (replay.numFrames >= FRAMES_PREALLOC || bufferPosition + _SCREEN_SIZE*_SCREEN_SIZE*sizeof(char) +  sizeof(uint32_t) + (sizeof(ReplayFrame)-sizeof(char*)) >= REPLAY_PREALLOC)
   {
        SerializeSection(&replay,false);
        replay.numFrames = 0;
        return;
   }
   else
   {
        memset(&replay.frames[replay.numFrames],0,sizeof(ReplayFrame));
   }




}
void PlayReplay(ALLEGRO_BITMAP* screen)
{
    //replay.framePlayPosition++;
    if (replay.framePlayPosition >= replay.numFrames)
    {
        //replay.framePlayPosition = 0;
        replay.playing = false;
        replay.framePlayPosition = replay.numFrames-1;
    }
    LoadFrame(screen,&replay.frames[replay.framePlayPosition]);
    for (int i = 0; i < NUM_SOUNDS_TO_SAVE; i++)
    {
        Sound* s = &replay.frames[replay.framePlayPosition].soundsPlayedThisFrame[i];
        if (s->path)
        {
            PlaySound(&replay.frames[replay.framePlayPosition].soundsPlayedThisFrame[i],1.0f,0);
        }
    }
}
//TODO: look into compressing this or appending it to an already compressed file
void SerializeSection(Replay* r, bool finished)
{
   r->totalFrames += r->numFrames;

    bool firstWrite = false;
   if (!tempFile)
   {
        remove("replays/" TEMP_REPLAY_NAME);
        tempFile = fopen("replays/" TEMP_REPLAY_NAME , "wb+");

        char* z = "\0";
        for (int i = 0; i < strlen(REPLAY_HEADER); i++)
            fwrite(&z,sizeof(char),1,tempFile);
        uint32_t frames = r->numFrames; uint32_t totalFrames = r->totalFrames;
        fwrite(&frames,sizeof(frames),1,tempFile);

   } 
   

   for (int i = 0; i < r->numFrames; i++)
   {
        ReplayFrame* rf = &r->frames[i];
        fwrite(&rf->numObjects, sizeof(rf->numObjects),1,tempFile);
        for (int i = 0; i < rf->numObjects; i++)
        {
            GameObject* g = &rf->objects[i];
            fwrite(&g->position.worldX, sizeof(g->position.worldX),1,tempFile);
            fwrite(&g->position.worldY, sizeof(g->position.worldY),1,tempFile);
            fwrite(&g->health, sizeof(g->health),1,tempFile);
            fwrite(&g->maxHP, sizeof(g->maxHP),1,tempFile);

            fwrite(&g->spriteIndex, sizeof(g->spriteIndex),1,tempFile);
        }
        fwrite(&rf->numAttacks, sizeof(rf->numAttacks),1,tempFile);
        for (int i = 0; i < rf->numAttacks; i++)
        {
            Attack* a = &rf->attacks[i];
            fwrite(&a->x, sizeof(a->x),1,tempFile);
            fwrite(&a->y, sizeof(a->y),1,tempFile);

            fwrite(&a->dither, sizeof(a->dither),1,tempFile);
            fwrite(&a->attackType, sizeof(a->attackType),1,tempFile);
            fwrite(&a->properties, sizeof(a->properties),1,tempFile);
    
            fwrite(&a->radius, sizeof(a->radius),1,tempFile);
        }
        fwrite(&rf->mapSpriteIndex,sizeof(rf->mapSpriteIndex),1,tempFile);


        
        char numSounds = 0;
        for (int i = 0; i < NUM_SOUNDS_TO_SAVE; i++)
        {
            if (rf->soundsPlayedThisFrame[i].path)
            {
                numSounds++;
            }
        }


        fwrite(&numSounds,sizeof(numSounds),1,tempFile);
        for (int i = 0; i < NUM_SOUNDS_TO_SAVE; i++)
        {
            if (rf->soundsPlayedThisFrame[i].path)
            {
                uint16_t len = strlen(rf->soundsPlayedThisFrame[i].path);
                fwrite(&len,sizeof(len),1,tempFile);
                fwrite(rf->soundsPlayedThisFrame[i].path,sizeof(char),len,tempFile);
            }
        }

        free(rf->objects);
        free(rf->attacks);
        rf->objects = NULL;
        rf->attacks = NULL;

   }

   if (finished)
   {
        //fclose(tempFile);
        //tempFile = fopen("replays/" TEMP_REPLAY_NAME , "r+b");
        rewind(tempFile);

        char* header = "REP";
        fwrite(header,sizeof(char),strlen(header),tempFile);

        fwrite(&r->totalFrames,sizeof(r->totalFrames),1,tempFile);
        fclose(tempFile);
        tempFile = NULL;
        return;

   }
   r->numFrames=1;
   r->framePlayPosition=0;

   bufferPosition = 0;


   

}


ReplayFrame SaveFrame(ALLEGRO_BITMAP* screen)
{
   // if (!frameTest)
    //{
    //    frameTest = malloc(_SCREEN_SIZE * _SCREEN_SIZE*(sizeof(char)+sizeof(uint16_t)));
   // }
    int numObjs = GetNumActiveObjects();
    ReplayFrame rf = {0};
    rf.numObjects = numObjs;
    rf.objects = calloc(rf.numObjects,sizeof(GameObject));
    int index = 0;
    for (int i = 0; i < numActiveObjects; i++)
    {
        //if (IsActive(activeObjects[i]))
        {
            rf.objects[index] = *activeObjects[i];
            index++;
        }
    }
    index = 0;

    rf.numAttacks = GetNumActiveAttacks();
    rf.attacks = calloc(rf.numAttacks,sizeof(Attack));
    
    for (int i = 0; i < MAX_ATTACKS; i++)
    {
        if (AttackIsActive(&attacks[i]))
        {
            rf.attacks[index] = attacks[i];
            index++;
        }
    }

    rf.mapSpriteIndex = currMap->spriteIndex;


    return rf;

    rf.compressedData =  &replayBuffer[bufferPosition];//malloc(_SCREEN_SIZE * _SCREEN_SIZE*(sizeof(char)+sizeof(uint16_t)));
    int toGetTo = _SCREEN_SIZE * _SCREEN_SIZE;
    int count = 0;
    ALLEGRO_LOCKED_REGION* region = al_lock_bitmap(screen,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    ALLEGRO_COLOR* pixels = region->data;
    int x=0; int y=0;

    int RLEPosition = 0;
    while (count < toGetTo)
    {
        //TODO: can be made faster by reading directly?
        ALLEGRO_COLOR pixel = al_get_pixel(screen,x,y);
        // al_unmap_rgb(pixels[count],&r,&g,&b);
        char pix = ALColorToCol(pixel);

        //count++;
        x++;
        if (x >= _SCREEN_SIZE)
        {
            x = 0; 
            y++;
        }
        int x2 = x; int y2 = y;
        for (int i = count; i < toGetTo; i++)
        {
            if (x2 >= _SCREEN_SIZE)
            {
                x2=0;
                y2++;
            }
            ALLEGRO_COLOR pixel2 = al_get_pixel(screen,x2,y2);
            if (pix != ALColorToCol(pixel2) || i == toGetTo-1)
            {
                uint16_t amt = i-count+1;
                count += amt;
                x=x2;
                y=y2;   
                memcpy(&rf.compressedData[RLEPosition],&amt,sizeof(uint16_t));
                RLEPosition += sizeof(uint16_t);
                memcpy(&rf.compressedData[RLEPosition],&pix,sizeof(char));
                RLEPosition += sizeof(char);
                break;
            }
            x2++;

        }


    }
    rf.dataLen = RLEPosition;
   al_unlock_bitmap(screen);
   int newSize;
   bufferPosition += RLEPosition;
   //rf.compressedData = realloc(rf.compressedData,rf.dataLen*sizeof(char));
    //RLECompressArray(colors,toGetTo,frameTest,&newSize);
    return rf;
}
//taken from http://zlib.net/zlib_how.html
int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
                assert(strm.avail_in == 0);     /* all input will be used */
                /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */


    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}
//taken from http://zlib.net/zlib_how.html
int inf(FILE *source, FILE *dest)
{

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];


    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
        
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;     /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
            }

            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            } 
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}
//taken from http://zlib.net/zlib_how.html  
void zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror(stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}
void RemoveReplay(Replay* r)
{
    if (!r) return;
    if (r->frames)
    {
        for (int i = 0; i < r->numFrames; i++)
        {
            ReplayFrame* rf = &r->frames[i];
            if (rf->objects)
                free(rf->objects);
            if (rf->attacks)
                free(rf->attacks);
           // if (rf->compressedData)
             //   free(rf->compressedData);
        }
        free(r->frames);
        r->frames = NULL;
    }

    if (replayBuffer)
        free(replayBuffer);
    replayBuffer = NULL;
    memset(r,0,sizeof(Replay));
    remove("replays/" TEMP_REPLAY_NAME);

    for (int i = 0; i < r->numSprites; i++)
    {
        al_destroy_bitmap(r->sprites[i].sprite);
    }
    if (r->sprites)
        free(r->sprites);
}

bool LoadReplay(char* path)
{
    FILE* f = fopen(path, "rb");
    if (f)
    {
        remove("replays/temp");
        FILE* temp = fopen("replays/temp","wb+");
        if (temp)
        {
            Replay* r = &replay;

            RemoveReplay(r);    
            inf(f,temp);
            fclose(temp);
            temp = fopen("replays/temp","rb");
            fseek(temp, 0, SEEK_END);
            int numBytes = ftell(temp);
            rewind(temp);
            
            fseek(temp,strlen(REPLAY_HEADER),SEEK_SET);
            fread(&r->totalFrames,sizeof(r->numFrames),1,temp);
            r->numFrames = r->totalFrames;
            r->frames = calloc(r->totalFrames,sizeof(ReplayFrame));
            for (int i = 0; i < r->totalFrames; i++)
            {
                ReplayFrame* rf = &r->frames[i];
                fread(&rf->numObjects,sizeof(rf->numObjects),1,temp);
                rf->objects = calloc(rf->numObjects,sizeof(GameObject));
                for (int j = 0; j < rf->numObjects; j++)
                {
                    GameObject* g = &rf->objects[j];
                    fread(&g->position.worldX,sizeof(g->position.worldX),1,temp);
                    fread(&g->position.worldY,sizeof(g->position.worldY),1,temp);
                    fread(&g->health,sizeof(g->health),1,temp);
                    fread(&g->maxHP,sizeof(g->maxHP),1,temp);
                    fread(&g->spriteIndex,sizeof(g->spriteIndex),1,temp);
                
                }
                fread(&rf->numAttacks,sizeof(rf->numAttacks),1,temp);
                rf->attacks = calloc(rf->numAttacks,sizeof(Attack));
                for (int j = 0; j < rf->numAttacks; j++)
                {
                    Attack* a = &rf->attacks[j];
                    fread(&a->x,sizeof(a->x),1,temp);
                    fread(&a->y,sizeof(a->y),1,temp);

                    fread(&a->dither,sizeof(a->dither),1,temp);
                    fread(&a->attackType,sizeof(a->attackType),1,temp);
                    fread(&a->properties,sizeof(a->properties),1,temp);
                    fread(&a->radius,sizeof(a->radius),1,temp);
                }
                fread(&rf->mapSpriteIndex,sizeof(rf->mapSpriteIndex),1,temp);


                char numSounds = 0;
                fread(&numSounds,sizeof(numSounds),1,temp);
                memset(rf->soundsPlayedThisFrame,0,NUM_SOUNDS_TO_SAVE*sizeof(Sound));
                for (int i = 0; i < numSounds; i++)
                {
                        uint16_t len = 0;
                        fread(&len,sizeof(len),1,temp);
                        rf->soundsPlayedThisFrame[i].path = calloc(len+1,sizeof(char));
                        fread(rf->soundsPlayedThisFrame[i].path,sizeof(char),len,temp);
                }


            }
            int32_t numSprRep = 0;
            fread(&numSprRep,sizeof(numSprRep),1,temp);

            r->sprites = calloc(numSprRep,sizeof(Sprite));
            r->numSprites = numSprRep;


            ALLEGRO_BITMAP* before = al_get_target_bitmap();
            for (int i = 0; i < numSprRep; i++)
            {
                uint32_t w = 0;
                uint32_t h = 0;

                fread(&w,sizeof(w),1,temp);
                fread(&h,sizeof(h),1,temp);

                r->sprites[i].sprite = al_create_bitmap(w,h);
                al_lock_bitmap(r->sprites[i].sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);
                al_set_target_bitmap(r->sprites[i].sprite);

                for (int x = 0; x < w; x++)
                {
                    for (int y = 0; y < h; y++)
                    {
                        ALLEGRO_COLOR pixel;
                        fread(&pixel,sizeof(ALLEGRO_COLOR),1,temp);
                        al_put_pixel(x,y,pixel);
                    }
                }
                al_unlock_bitmap(r->sprites[i].sprite);

            }
            al_set_target_bitmap(before);



            /*char* repBuffer = calloc(numBytes,sizeof(char));
            fread(repBuffer, 1, numBytes, temp);
            
            if (replayBuffer)
                replayBuffer = realloc(replayBuffer,numBytes*sizeof(char));
            else
                replayBuffer = calloc(numBytes,sizeof(char));

            char header[3];
            int byteCount = 3;
            if (numBytes >= byteCount)
            {
                if (strncmp(repBuffer,"REP",3) != 0)
                {
                    return false;
                }
            }
            byteCount += sizeof(r->totalFrames);
            if (numBytes >= byteCount)
            {
                memcpy(&replay.totalFrames,&repBuffer[byteCount-sizeof(r->totalFrames)],sizeof(r->totalFrames));
                replay.numFrames = replay.totalFrames;
            }
            r->frames = calloc(r->totalFrames,sizeof(ReplayFrame));
            
            memcpy(replayBuffer,repBuffer,sizeof(char) * (numBytes));

            int position = 7;
            for (int i = 0; i < r->totalFrames; i++)
            {

                ReplayFrame* rf = &r->frames[i];
                memset(rf,0,sizeof(ReplayFrame));
                byteCount += sizeof(rf->dataLen);
                if (numBytes >= byteCount)
                {
                    memcpy(&rf->dataLen,&repBuffer[byteCount-sizeof(rf->dataLen)],sizeof(rf->dataLen));
                }
                else
                {
                    return false;
                }
                byteCount += rf->dataLen;

                if (numBytes >= rf->dataLen)
                {
                    rf->compressedData = &replayBuffer[byteCount-rf->dataLen];//calloc(rf->dataLen+1,sizeof(char));
                    position += rf->dataLen;
                }
                else
                {
                    return false;
                }

                byteCount += sizeof(char);
                char soundLen;
                if (numBytes >= byteCount)  
                {
                    memcpy(&soundLen,&repBuffer[byteCount-sizeof(char)],sizeof(char));
                }
                else
                {
                    return false;
                }

                int index = 0;
                for (int i = 0; i < soundLen; i++)
                {
                    uint16_t pathLen;
                    byteCount += sizeof(pathLen);
                    if (numBytes >= byteCount)
                    {
                        memcpy(&pathLen,&repBuffer[byteCount-sizeof(pathLen)],sizeof(pathLen));
                    }
                    else
                    {
                        return false;
                    }
                    byteCount += pathLen;
                    if (numBytes >= byteCount)
                    {
                        Sound* s = &rf->soundsPlayedThisFrame[index++];
                        char* soundPath = calloc(pathLen+1,sizeof(char));
                        
                        memcpy(soundPath,&repBuffer[byteCount-pathLen],sizeof(char) * pathLen);

                        *s = sounds[LoadSound(soundPath)];
                        free(soundPath);
                    }
                    else
                    {
                        return false;
                    }


                }



            }*/
            remove("replays/temp");
        }
    }
    else
        return false;

    replay.playing = true;
    return true;
    
}
void SaveSpriteReplay(FILE* f, Sprite* s)
{
    uint32_t w = GetWidthSprite(s);
    uint32_t h = GetHeightSprite(s);

    fwrite(&w,sizeof(w),1,f);
    fwrite(&h,sizeof(h),1,f);

    al_lock_bitmap(s->sprite,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            ALLEGRO_COLOR pixel = al_get_pixel(s->sprite,x,y);
            fwrite(&pixel,sizeof(ALLEGRO_COLOR),1,f);
        }
    }
    al_unlock_bitmap(s->sprite);
}
void SaveReplaySpriteTable(FILE* f, FILE* to)
{

    int32_t spritesIndices[numSprites];
    memset(spritesIndices,0,numSprites*sizeof(int32_t));


    GameObject* g = &objects[0];
    int offset = strlen(REPLAY_HEADER) + sizeof(uint32_t);
    for (int i = 0; i < replay.totalFrames; i++)
    {
        fseek(f,offset,SEEK_SET);

        int32_t numObjects; 
        fread(&numObjects,sizeof(numObjects),1,f);
        offset += sizeof(numObjects);

        for (int j = 0; j < numObjects; j++)
        {
            const int positionOffset = sizeof(g->position.worldX) + sizeof(g->position.worldY) + sizeof(g->health) + sizeof(g->maxHP);
            fseek(f, offset + positionOffset, SEEK_SET);

            uint32_t spriteIndex = 0;
            fread(&spriteIndex,sizeof(g->spriteIndex),1,f);

            spritesIndices[spriteIndex] = spriteIndex;

            offset += positionOffset + sizeof(g->spriteIndex);
        }  
        ReplayFrame* rf = &replay.frames[0];
        Attack* a = &rf->attacks[0];

        int32_t numAttacks = 0;
        fread(&numAttacks,sizeof(rf->numAttacks),1,f);

        int attackOffset = sizeof(rf->numAttacks) + ((sizeof(a->x) + sizeof(a->y) + sizeof(a->dither) + sizeof(a->attackType) + sizeof(a->properties) + sizeof(a->radius)) * numAttacks);
        offset += attackOffset;

        spritesIndices[rf->mapSpriteIndex] = rf->mapSpriteIndex;


        int32_t mapSprite=0;
        offset += sizeof(rf->mapSpriteIndex);


        fseek(f,offset,SEEK_SET);
        int numSounds = 0;
        fread(&numSounds,sizeof(numSounds),1,f);
        offset += sizeof(numSounds);

        memset(rf->soundsPlayedThisFrame,0,NUM_SOUNDS_TO_SAVE*sizeof(Sound));
        for (int i = 0; i < numSounds; i++)
        {
            uint16_t len = 0;
            fread(&len,sizeof(len),1,f);
            offset += sizeof(len);
            offset += len;
            fseek(f,offset,SEEK_SET);
        }


        //fseek(f,offset,SEEK_SET);
        //char numSounds = 0; 

        //int soundOffset = sizeof(rf->numSounds) + ((sizeof(a->x) + sizeof(a->y) + sizeof(a->dither) + sizeof(a->attackType) + sizeof(a->properties) + sizeof(a->radius)) * numAttacks);


       // offset += sizeof(numSounds);
    }

    fseek(to,0,SEEK_END);
    
    fwrite(&numSprites,sizeof(numSprites),1,to);
    for (int i = 0; i < numSprites; i++)
    {
        SaveSpriteReplay(to,&sprites[spritesIndices[i]]); 
    }

    
}
void ReplayToDisk(Replay* r)
{
    //zlib to compress
    r->framePlayPosition = 0;
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    size_t buffsiz = snprintf(NULL,0, "replays/%d%02d%02d_%02d%02d%02d_.rep", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    char* filename = calloc(buffsiz+1,sizeof(char));
    sprintf(filename,"replays/%d%02d%02d_%02d%02d%02d_.rep", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    

   // FILE* f = fopen(filename, "wb+");
    //fwrite(header,sizeof(char),strlen(header),f);
    //fwrite(&r->numFrames,sizeof(r->totalFrames),1,f);
        /*
    for (int i = 0; i < r->numFrames; i++)
    {
        ReplayFrame* rf = &r->frames[i];
        fwrite(&rf->dataLen,sizeof(rf->dataLen),1,f);
        fwrite(rf->compressedData,sizeof(char),rf->dataLen,f);
        
        char numSounds = 0;
        for (int i = 0; i < NUM_SOUNDS_TO_SAVE; i++)
        {
            if (rf->soundsPlayedThisFrame[i].path)
            {
                numSounds++;
            }
        }
        fwrite(&numSounds,sizeof(numSounds),1,f);
        for (int i = 0; i < NUM_SOUNDS_TO_SAVE; i++)
        {
            if (rf->soundsPlayedThisFrame[i].path)
            {
                uint32_t len = strlen(rf->soundsPlayedThisFrame[i].path);
                fwrite(&len,sizeof(len),1,f);
                fwrite(rf->soundsPlayedThisFrame[i].path,sizeof(char),len,f);
            }
        }

    }
    fclose(f);
    */

    FILE* f = fopen("replays/" TEMP_REPLAY_NAME, "r+b");
    fseek(f,strlen(REPLAY_HEADER),SEEK_SET);
    fwrite(&replay.totalFrames,sizeof(replay.totalFrames),1,f);
    fseek(f,0,SEEK_SET);

    fseek(f,0,SEEK_END);
    SaveReplaySpriteTable(f,f);
    fseek(f,0,SEEK_SET);



    char* compressedFilename = calloc(buffsiz+1,sizeof(char));
    sprintf(compressedFilename,"replays/%d%02d%02d_%02d%02d%02d.rep", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    FILE* compressed = fopen(compressedFilename, "wb+");



    SET_BINARY_MODE(f);
    SET_BINARY_MODE(compressed);

    int ret = def(f,compressed,9);
    if (ret != Z_OK)
        zerr(ret);
    remove(filename);

    fclose(compressed);
    fclose(f);

    free(filename);
    free(compressedFilename);

    remove("replays/" TEMP_REPLAY_NAME);

}