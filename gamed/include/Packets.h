/*
IntWars playground server for League of Legends protocol testing
Copyright (C) 2012  Intline9 <Intline9@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _PACKETS_H
#define _PACKETS_H

#include <time.h>
#include <cmath>

#include <intlib/general.h>
#include <enet/enet.h>

#include "common.h"
#include "Buffer.h"
#include "Client.h"

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

class Packet {
protected:
   Buffer buffer;
   
public:
   const Buffer& getBuffer() const { return buffer; }
   Packet(uint8 cmd = 0) {
      buffer << cmd;
   }

};

class BasePacket : public Packet {
   
public:
   BasePacket(uint8 cmd = 0, uint32 netId = 0) : Packet(cmd) {
      buffer << netId;
   }

};

class GamePacket : public BasePacket {
   
public:
   GamePacket(uint8 cmd = 0, uint32 netId = 0, uint32 ticks = 0) : BasePacket(cmd, netId) {
      buffer << ticks;
   }

};


struct PacketHeader {
    PacketHeader() {
        netId = 0;
    }

    PacketCmd cmd;
    uint32 netId;
};

struct GameHeader {
    GameHeader() {
        netId = ticks = 0;
    }

    GameCmd cmd;
    uint32 netId;
    uint32 ticks;
};
typedef struct _SynchBlock {
    _SynchBlock() {
        userId = 0xFFFFFFFFFFFFFFFF;
        netId = 0x1E;
        teamId = 0x64;
        unk2 = 0;
        flag = 0; //1 for bot?
        memset(data1, 0, 64);
        memset(data2, 0, 64);
        unk3 = 0x19;
    }

    uint64 userId;
    uint16 netId;
    uint32 skill1;
    uint32 skill2;
    uint8 flag;
    uint32 teamId;
    uint8 data1[64];
    uint8 data2[64];
    uint32 unk2;
    uint32 unk3;
} SynchBlock;

struct ClientReady {
    uint32 cmd;
    uint32 playerId;
    uint32 teamId;
};

typedef struct _SynchVersionAns {
    _SynchVersionAns() {
        header.cmd = PKT_S2C_SynchVersion;
        ok = ok2 = 1;
        memcpy(version, "Version 4.12.0.356 [PUBLIC]", 27);
        memcpy(gameMode, "CLASSIC", 8);
        memset(zero, 0, 2232);
        end1 = 0xE2E0;
        end2 = 0xA0;
    }

    PacketHeader header;
    uint8 ok;
    uint32 mapId;
    SynchBlock players[12];
    uint8 version[27];      //Ending zero so size 26+0x00
    uint8 ok2;              //1
    uint8 unknown[228];     //Really strange shit
    uint8 gameMode[8];
    uint8 zero[2232];
    uint16 end1;            //0xE2E0
    uint8 end2;             //0xA0 || 0x08
} SynchVersionAns;

typedef struct _PingLoadInfo {
    PacketHeader header;
    uint32 unk1;
    uint64 userId;
    float loaded;
    float ping;
    float f3;
} PingLoadInfo;

uint8 *createDynamicPacket(uint8 *str, uint32 size);

typedef struct _LoadScreenInfo {
    _LoadScreenInfo() {
        //Zero this complete buffer
        memset(this, 0, sizeof(_LoadScreenInfo));
        cmd = PKT_S2C_LoadScreenInfo;
        blueMax = redMax = 6;
    }

    uint8 cmd;
    uint32 blueMax;
    uint32 redMax;
    uint64 bluePlayerIds[6]; //Team 1, 6 players max
    uint8 blueData[144];
    uint64 redPlayersIds[6]; //Team 2, 6 players max
    uint8 redData[144];
    uint32 bluePlayerNo;
    uint32 redPlayerNo;
} LoadScreenInfo;

class LoadScreenPlayerName : public Packet {
public:
   LoadScreenPlayerName(const ClientInfo& player) : Packet(PKT_S2C_LoadName) {
      buffer << player.userId;
      buffer << (uint32)0;
      buffer << (uint32)player.getName().length()+1;
      buffer << player.getName();
      buffer << (uint8)0;
   }

    /*uint8 cmd;
    uint64 userId;
    uint32 skinId;
    uint32 length;
    uint8* description;*/
};

class LoadScreenPlayerChampion : public Packet {
public:
   LoadScreenPlayerChampion(const ClientInfo& player) : Packet(PKT_S2C_LoadHero) {
      buffer << player.userId;
      buffer << player.skinNo;
      buffer << (uint32)player.getChampion()->getType().length()+1;
      buffer << player.getChampion()->getType();
      buffer << (uint8)0;
   }

    /*uint8 cmd;
    uint64 userId;
    uint32 skinId;
    uint32 length;
    uint8* description;*/
};


typedef struct _KeyCheck {
    _KeyCheck() {
        cmd = PKT_KeyCheck;
        playerNo = 0;
        checkId = 0;
        trash = trash2 = 0;
    }

    uint8 cmd;
    uint8 partialKey[3];   //Bytes 1 to 3 from the blowfish key for that client
    uint32 playerNo;
    uint64 userId;         //uint8 testVar[8];   //User id
    uint32 trash;
    uint64 checkId;        //uint8 checkVar[8];  //Encrypted testVar
    uint32 trash2;
} KeyCheck;

typedef struct _AttentionPing {
    _AttentionPing() {
    }
    _AttentionPing(_AttentionPing *ping) {
        cmd = ping->cmd;
        unk1 = ping->unk1;
        x = ping->x;
        y = ping->y;
        z = ping->z;
        unk2 = ping->unk2;
        type = ping->type;
    }

    uint8 cmd;
    uint32 unk1;
    float x;
    float y;
    float z;
    uint32 unk2;
    uint8 type;
} AttentionPing;

typedef struct _AttentionPingAns {
    _AttentionPingAns(AttentionPing *ping) {
        attentionPing = AttentionPing(ping);
        attentionPing.cmd = PKT_S2C_AttentionPing;
    }

    AttentionPing attentionPing;
    uint32 response;
} AttentionPingAns;

struct CameraLock {
    PacketHeader header;
    uint8 isLock;
    uint32 padding;
};

typedef struct _ViewReq {
    uint8 cmd;
    uint32 unk1;
    float x;
    float zoom;
    float y;
    float y2;		//Unk
    uint32 width;	//Unk
    uint32 height;	//Unk
    uint32 unk2;	//Unk
    uint8 requestNo;
} ViewReq;

struct CastSpell {
    PacketHeader header;
    uint8 spellSlot;
    float x, y;
    float x2, y2;
    uint32 targetNetId; // If 0, use coordinates, else use target net id
};

struct CastSpellAns {

   CastSpellAns(uint32 casterNetId, float x, float y) : casterNetId(casterNetId), casterNetId2(casterNetId), unk2(0x400001f59c0cb5a7), x(x), z(55), y(y), x2(x), z2(55), y2(y), finalX(x), finalZ(55), finalY(y), unk4_3(0x41e0), unk5(1) {
      header.cmd = (GameCmd)PKT_S2C_CastSpellAns;
      header.netId = casterNetId;
      header.ticks = clock();
      memcpy(unk, "\x00\x66\x00\x44\x40\x7f\x01\xf6\x01\x00\x40\x00\x00\x00\x80\x3f", 16);
      castTime = 0.25f;
      unk3_2 = 0;
      unk3_3 = 1.0f;
      cooldown = 6.0f;
      unk3_4 = 28.0f;
      spellSlot = 0;
   }
   
   GameHeader header;
   uint8 unk[16];
   uint32 casterNetId, casterNetId2;
   uint64 unk2;
   float x, z, y;
   float x2, z2, y2;
   uint8 unk3;
   float castTime; // Unsure - 0.25 for ez Q
   float unk3_2; // 0 for ez Q
   float unk3_3; // 1.0 for ez Q ?
   float cooldown; // 6.0 for ez Q
   float unk3_4; // 28.0 for ez Q ?
   uint8 unk4_1;
   uint8 spellSlot;
   short unk4_2;
   short unk4_3;
   float finalX, finalZ, finalY;
   uint64 unk5;
};

/**
 * Change Target ??
 */
struct Unk {
   Unk(uint32 netId, float x, float y, uint32 targetNetId = 0) : unk1(0x0F), unk2(1), unk3(2), x(x), unk4(55), y(y), targetNetId(targetNetId) {
      header.cmd = PKT_S2C_UNK;
      header.netId = netId;
   }

   PacketHeader header;
   uint8 unk1, unk2, unk3;

   float x, unk4, y;
   uint32 targetNetId;
};

enum MinionSpawnPosition : uint32 {
   SPAWN_BLUE_TOP = 0xeb364c40,
   SPAWN_BLUE_BOT = 0x53b83640,
   SPAWN_BLUE_MID = 0xb7717140,
   SPAWN_RED_TOP  = 0xe647d540,
   SPAWN_RED_BOT  = 0x5ec9af40,
   SPAWN_RED_MID  = 0xba00e840
};

enum MinionSpawnType : uint32 {
   MINION_TYPE_MELEE = 0x00,
   MINION_TYPE_CASTER = 0x01,
   MINION_TYPE_CANNON = 0x02
};

struct MinionSpawn {
   
   MinionSpawn(uint32 netId, uint32 position = SPAWN_RED_MID, uint8 type = MINION_TYPE_MELEE) : netId(netId), netId2(netId), netId3(netId), unk(0x00150017), unk2(0x03), position(position), unk4(0xff), unk5_1(1), type(type), unk5_3(0), unk5_4(1), unk7(5), unk8(0x0ff84540f546f424) {
      header.cmd = PKT_S2C_MinionSpawn;
      header.netId = netId;
      memcpy(unk6, "\x0a\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x3f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x2c\x27\x00\x00\x06", 36);
   }

   PacketHeader header;
   
   uint32 unk;
   uint8 unk2;
   uint32 netId, netId2;
   uint32 position;
   uint8 unk4;
   uint8 unk5_1;
   uint8 type;
   uint8 unk5_3;
   uint8 unk5_4;
   uint8 unk6[36];
   uint32 netId3;
   uint8 unk7;
   uint64 unk8;
};

struct MovementVector {
    short x;
    short y;
    MovementVector() {
        x = 0;
        y = 0;
    }
    MovementVector(uint16 x, uint16 y) {
        this->x = x;
        this->y = y;
    }
};

struct MovementReq {
    PacketHeader header;
    MoveType type;
    float x;
    float y;
    uint32 zero;
    uint8 vectorNo;
    uint32 netId;
    uint8 moveData;
};

struct MovementAns {
    MovementAns() {
        header.cmd = PKT_S2C_MoveAns;
    }

    GameHeader header;
    uint16 ok;
    uint8 vectorNo;
    uint32 netId;
    uint8 moveData; //bitMasks + Move Vectors

    MovementVector *getVector(uint32 index) {
        if(index >= vectorNo / 2)
        { return NULL; }
        MovementVector *vPoints = (MovementVector *)(&moveData + maskCount());
        return &vPoints[index];
    }

    int maskCount() {
        float fVal = vectorNo / 2;
        return std::ceil((fVal - 1) / 4);
    }

    static uint32 size(uint8 vectorNo) {
        float fVectors = vectorNo;
        int maskCount = std::ceil((fVectors - 1) / 4);
        return sizeof(MovementAns) + (vectorNo * sizeof(MovementVector)) + maskCount; //-1 since struct already has first moveData byte
    }

    uint32 size() {
        return size(vectorNo / 2);
    }

    static MovementAns *create(uint32 vectorNo) {
        int nSize = size(vectorNo / 2);
        MovementAns *packet = (MovementAns *)new uint8[nSize];
        memset(packet, 0, nSize);
        packet->header.cmd = PKT_S2C_MoveAns;
        packet->header.ticks = clock();
        packet->vectorNo = vectorNo;
        return packet;
    }

    static void destroy(MovementAns *packet) {
        delete [](uint8 *)packet;
    }

};

typedef struct _ViewAns {
    _ViewAns() {
        cmd = PKT_S2C_ViewAns;
        unk1 = 0;
    }

    uint8 cmd;
    uint32 unk1;
    uint8 requestNo;
} ViewAns;


typedef struct _QueryStatus {
    _QueryStatus() {
        header.cmd = PKT_S2C_QueryStatusAns;
        ok = 1;
    }
    PacketHeader header;
    uint8 ok;
} QueryStatus;

typedef struct _SynchVersion {
    PacketHeader header;
    uint32 unk1;
    uint32 unk2;
    uint8 version[50]; //Dunno how big and when usefull data begins
} SynchVersion;

typedef struct _WorldSendGameNumber {
    _WorldSendGameNumber() {
        header.cmd = PKT_World_SendGameNumber;
        memset(data, 0, sizeof(data1));
        memset(data, 0, sizeof(data));
        gameId = 0;
    }

    PacketHeader header;
    uint64 gameId; //_0x0000
    uint8 server[5]; //0x0008
    uint8 data1[27]; //0x000D
    uint8 data[0x80];//0x0028
} WorldSendGameNumber;


struct CharacterStats {

   CharacterStats(uint8 masterMask, uint32 netId, uint32 mask, float value) : updateNo(1), masterMask(masterMask), netId(netId), mask(mask), size(4) {
      header.cmd = (GameCmd)PKT_S2C_CharStats;
      header.ticks = clock();
      this->value.fValue = value;
   }
   
   CharacterStats(uint8 masterMask, uint32 netId, uint32 mask, unsigned short value) : updateNo(1), masterMask(masterMask), netId(netId), mask(mask), size(2) {
      header.cmd = (GameCmd)PKT_S2C_CharStats;
      header.ticks = clock();
      this->value.sValue = value;
   }

   GameHeader header;
   uint8 updateNo;
   uint8 masterMask;
   uint32 netId;
   uint32 mask;
   uint8 size;
   union {
      unsigned short sValue;
      float fValue;
   } value;
};

struct ChatMessage {
    uint8 cmd;
    uint32 netId;
    uint32 unk1;
    uint8 unk2;

    uint32 playerNo;
    ChatType type;
    uint32 lenght;
    uint8 unk3[32];
    int8 msg;

    int8 *getMessage() {
        return &msg;
    }
};

typedef struct _UpdateModel {
    _UpdateModel(uint32 netID, const char *szModel) {
        memset(this, 0, sizeof(_UpdateModel));
        header.cmd = (PacketCmd)0x97;
        header.netId = netID;
        id = netID & ~0x40000000;
        bOk = 1;
        unk1 = -1;
        strncpy((char *)szName, szModel, 32);
    }
    PacketHeader header;
    uint32 id;
    uint8 bOk;
    uint32 unk1;
    uint8 szName[32];
} UpdateModel;
typedef struct _StatePacket {
    _StatePacket(PacketCmd state) {
        header.cmd = state;
    }
    PacketHeader header;
} StatePacket;
typedef struct _StatePacket2 {
    _StatePacket2(PacketCmd state) {
        header.cmd = state;
        nUnk = 0;
    }
    PacketHeader header;
    int16 nUnk;
} StatePacket2;

struct FogUpdate2 {
    FogUpdate2() {
        header.cmd = PKT_S2C_FogUpdate2;
        header.netId = 0x40000019;
    }
    PacketHeader header;
    float x;
    float y;
    uint32 radius;
    uint8 unk1;
};

struct Click {

   PacketHeader header;
   uint32 zero;
   uint32 targetNetId; // netId on which the player clicked

};

struct Unk2 {
   Unk2(uint32 sourceNetId, uint32 targetNetId)  : targetNetId(targetNetId) {
      header.cmd = PKT_S2C_Unk2;
      header.netId = sourceNetId;
   }

   PacketHeader header;
   uint32 targetNetId;
};

struct HeroSpawn {
    HeroSpawn() {
        header.cmd = PKT_S2C_HeroSpawn;
        unk1 = 0;
        memset(&name, 0, 128 + 64); //Set name + type to zero
        x = 130880;
        y = 502;
    }

    PacketHeader header;
    uint32 netId; //Also found something about locking flag//Perhaps first 4 bits is type and rest is netId?? or something?? //Linked for mastery's (first uitn32, and also animation (looks like it) and possible more) often looks like XX 00 00 40 where XX is around 10-30
    uint32 gameId; //1-number of players
    uint32 x;       //Some coordinates, no idea how they work yet
    uint32 y;
    uint16 unk1;
    uint8 name[128];
    uint8 type[64];
} ;
struct HeroSpawn2 {
    HeroSpawn2() {
        header.cmd = (PacketCmd)0xBA;
        memset(unk, 0, 30);
        unk[15] = 0x80;
        unk[16] = 0x3F;
        unk1 = 3;
        unk2 = 1;
        f1 = 0x420F9C78;
        f2 = 0x4388C6A5;
        f3 = 0x3F441B7D;
        f4 = 0x3F248DBB;
    }

    PacketHeader header;
    uint8 unk[30];
    uint8 unk1;
    uint32 unk2;
    uint32 f1;
    uint32 f2;
    uint32 f3;
    uint32 f4;
};
struct HeroSpawn3 {
    HeroSpawn3() {
        header.cmd = (PacketCmd)0xAE;
        unk = 0;
        health = 1337;
        maxHealth = 666;
    }

    PacketHeader header;
    uint16 unk;
    float health;
    float maxHealth;
};

struct TurretSpawn {
    TurretSpawn() {
        header.cmd = PKT_S2C_TurretSpawn;
        tID = 0;
        memset(&name, 0, 29 + 42); //Set name + type to zero
    }

    PacketHeader header;
    uint32 tID;
    uint8 name[28];
    uint8 type[42];
};
struct GameTimer {
    GameTimer(float fTime) {
        header.cmd = PKT_S2C_GameTimer;
        header.netId = 0;
        this->fTime = fTime;
    }
    PacketHeader header;
    float fTime;
};
struct GameTimerUpdate {
    GameTimerUpdate(float fTime) {
        header.cmd = PKT_S2C_GameTimerUpdate;
        header.netId = 0;
        this->fTime = fTime;
    }
    PacketHeader header;
    float fTime;
};
struct SpellSet {
    SpellSet(uint32 netID, uint32 _spellID, uint32 _level) {
        memset(this, 0, sizeof(SpellSet));
        header.cmd = PacketCmd(0x5A);
        header.netId = netID;
        spellID = _spellID;
        level = _level;
    }
    PacketHeader header;
    uint32 spellID;
    uint32 level;
};
class LevelPropSpawn : public BasePacket {
    public:
        LevelPropSpawn(uint32 netId, const std::string& name, const std::string& type, float x, float y, float z) : BasePacket(PKT_S2C_LevelPropSpawn) {
            buffer << netId;
            buffer << (uint32)0x00000040; // unk
            buffer << (uint8)0; // unk
            buffer << x << z << y;
            buffer.fill(0, 41); // unk
            buffer << name;
            buffer.fill(0, 64-name.length());
            buffer << type;
            buffer.fill(0, 64-type.length());
        }
        
        /*PacketHeader header;
        uint32 netId;
        uint32 unk1;
        uint8 unk2;
        float x;
        float y;
        float z; // unsure
        uint8 unk3[41];
        uint8 name[64];
        uint8 type[64];*/
};

struct Announce {
    PacketHeader header;
    uint8 msg;
    uint64 unknown;
    uint32 mapNo;
};

typedef struct _SkillUpPacket {
    PacketHeader header;
    uint8 skill;
} SkillUpPacket;

class SkillUpResponse : public BasePacket {
public:
    SkillUpResponse(uint32 netId, uint8 skill, uint8 level, uint8 pointsLeft) : BasePacket(PKT_S2C_SkillUp, netId) {
        buffer << skill << level << pointsLeft;
    }
};

typedef struct _BuyItemReq {
    PacketHeader header;
    uint32 id;
} BuyItemReq;

typedef struct _BuyItemAns {
    _BuyItemAns() {
        header.cmd = (PacketCmd)PKT_S2C_BuyItemAns;
        unk1 = 0;
        unk2 = 0;
        unk3 = 0;
    }
    PacketHeader header;
    uint16 itemId;
    uint16 unk1;
    uint8 slotId;
    uint8 stack;
    uint16 unk2;
    uint8 unk3;
} BuyItemAns;

typedef struct _EmotionPacket {
    PacketHeader header;
    uint8 id;
} EmotionPacket;

typedef struct _EmotionResponse {
    _EmotionResponse() {
        header.cmd = PKT_S2C_Emotion;
    }
    PacketHeader header;
    uint8 id;
} EmotionResponse;
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

#endif