#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{

    enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END, DIM_LB_Release };
    enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

    enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };

    enum BONE_TYPE { BONE, DYNAMIC_BONE };

    enum EVENT_FRAME_TYPE
    {
        EVENT_SOUND,
        EVENT_EFFECT,
        EVENT_COLLIDER,
        EVENT_STATE,
        EVENT_END,
    };


    enum GROUP_TYPE
    {
        PLAYER = 1,				  /* 0x0000001 */
        PLAYER_WEAPON = 1 << 1,   /* 0x0000010 */
        MONSTER = 1 << 2,		  /* 0x0000100 */
        MONSTER_WEAPON = 1 << 3,  /* 0x0001000 */
        ENEMY = 1 << 4,		  /* 0x0010000 */
        OBJECT = 1 << 5,		  /* 0x0100000 */
        ITEM = 1 << 6,			/* 0x1000000 */
        DESTRUCT = 1 << 7,			/* 0x1000000 */
        GROUP_END = 1 << 8,
    };

    enum class EVENT_TYPE {
        EVENT_1,
        EVENT_2,
        EVENT_3,
        EVENT_4,
        EVENT_5,
    };

    enum CHANNELID
    {
        SOUND_BGM,
        SOUND_ACTION,
        SOUND_EFFECT,
        SOUND_MOREEFFECT,
        SOUND_SURROUNDING,
        SOUND_NATURAL,
        SOUND_STRUCT,
        SOUND_ElectricEel,
        SOUND_ElectricEel_Collision,
        SOUND_RHINO,
        SOUND_STONE,
        SOUND_PLAYER_ATTACK_1,  
        SOUND_PLAYER_ATTACK_2,  
        SOUND_PLAYER_PARRY_1,   
        SOUND_PLAYER_PARRY_2,   
        SOUND_PLAYER_ACTION_1,  
        SOUND_PLAYER_ACTION_2,  
        SOUND_MONSTER_STUN, 
        SOUND_BOSS_ACTION,
        SOUND_MONSTER_ACTION,
        SOUND_MONSTER_VOICE,
        SOUND_MONSTER_WEAPON,
        SOUND_MONSTER_DAMAGE,
        SOUND_NPC,
        SOUND_UI,
        MAXCHANNEL = 32,
    };
    enum UIOBJTYPE
    {
        UI_BUTTON,
        UI_BUTTONPLAYER,
        UI_IMAGE,
        UI_TEXT,
        UI_TEXTPLAYER,
        UI_END
    };
    enum TextDrawEffect
    {
        TEXT_DEFALUT,
        TEXT_SHADOW,
        TEXT_OUTLINE,
        TEXT_TWOCOLOR,
        TEXT_ALPHA,
        TEXT_ALPHALOOP,
        TEXT_ALPHA_ANIM,
        FONT_END
    };

    enum class BINARY {
        BINARY_SAVE,
        BINARY_LOAD,
        BINARY_END,
    };

    enum class COLLIDER_TYPE
    {
        COLLIDER_BOX,
        COLLIDER_CAPSULE,
        COLLIDER_SPHERE,
        COLLIDER_Field,
    };

    enum class FRUSTUM_TYPE
    {
        FRUSTUM_OBJECT,
        FRUSTUM_MONSTER,
        FRUSTUM_END,

    };


    enum Player_Hitted_State
    {
        PLAYER_HURT_KNOCKDOWN,			     // 캐릭터 넉다운
        PLAYER_HURT_HURTMFL,			     // 조금 뒤로 이동하면서 휘청 
        PLAYER_HURT_HURTSF,				     // 아주 조금 뒤로 이동하면서 휘청 
        PLAYER_HURT_HURTSL,				     // 아주 조금 뒤로 이동하면서 휘청 
        PLAYER_HURT_HURTLF,				     // 보통 길게 뒤로 이동하면서 휘청 하면서 무릎 꿇음	
        PLAYER_HURT_HURXXLF,			     // 보통 길게 뒤로 이동하면서 휘청 하면서 무릎 꿇는 시간 조금 김 
        PLAYER_HURT_KnockBackF,			     // 길게 뒤로 밀리면서 한손으로 땅짚고 일어남
        PLAYER_HURT_FallDown,			     // 공중에 띄워지면서 날라감
        PLAYER_HURT_STUN,			         // 스턴걸기
        PLAYER_HURT_REBOUND,                 // 몬스터가 패링했을때
        PLAYER_HURT_CATCH,                   // 몬스터한테 잡혔을 때
        PLAYER_HURT_MAGICIAN_CATCH,          // 마술사의 스킬에 당할 때	
        PLAYER_HURT_MUTATION_MAGICIAN_CATCH, // 변이 마술사에게 스킬을 당할 때	
        PLAYER_HURT_RESEARCH_CATCH,          // 리서처에게 잡혔을 때		
        PLAYER_HURT_END,
    };


    enum PLAYER_SKILL
    {
        PLAYER_SKILL_START,
        PLAYER_SKILL_AXE,
        PLAYER_SKILL_BLOODWHIP,
        PLAYER_SKILL_BOW,
        PLAYER_SKILL_CANESWORD,
        PLAYER_SKILL_DAGGER,
        PLAYER_SKILL_FISTBLADE,
        PLAYER_SKILL_GREADSWORD,
        PLAYER_SKILL_HALBERD,
        PLAYER_SKILL_HAMMER,
        PLAYER_SKILL_KNIFE,
        PLAYER_SKILL_MAGICIANLV2,
        PLAYER_SKILL_PUPPET,
        PLAYER_SKILL_SCYTHE,
        PLAYER_SKILL_SHIELD,
        PLAYER_SKILL_TWINSWORD,
        PLAYER_SKILL_URDSWORD,
        PLAYER_SKILL_VARGSWORD,
        PLAYER_SKILL_JAVELINSWORD,
        PLAYER_SKILL_END,
    };

    enum MONSTER_STATE
    {
        STATE_IDLE,
        STATE_MOVE,
        STATE_RUN,
        STATE_ATTACK,
        STATE_HIT,
        STATE_STUN,
        STATE_PARRY,
        STATE_PARRY_ATTACK,
        STATE_EXECUTION,
        STATE_DEAD,
        STATE_INTRO,
        STATE_SPECIAL_ATTACK,
        STATE_SPECIAL_ATTACK2,
        STATE_KICK_ATTACK, //나중에 킥이 아닌 무기가 아닌 걸로 공격한걸로 의미하는걸로 바꿔야할듯
        STATE_END
    };

    enum MONSTER_EXECUTION_CATEGORY
    {
        MONSTER_START = 0,
        MONSTER_VARG,               //바그	
        MONSTER_HARMOR,             //검병	
        MONSTER_JOKER,              //조커	
        MONSTER_VILLAGEM1,          //방패병
        MONSTER_MAGICIAN,           // 오두르	
        MONSTER_MUTATION_MAGICIAN,	// 변이된 오두르
        MONSTER_PUNCH_MAN,          // 펀치맨
        MONSTER_GRACE,               // 그레이스
        MONSTER_URD,				// URD 보스 
        MONSTER_BAT,				// Bat 보스 ( 박쥐 ) 
        MONSTER_RESEARCHER,         // 리서처 엘리트 몹	
        MONSTER_NORMAL              //기본몬스터	
    };

    enum MONSTER_CATEGORY
    {
        CATEGORY_NORMAL,
        CATEGORY_ELITE,
        CATEGORY_BOSS,
        CATEGORY_END
    };

    enum PROJECTILE_CATEGORY
    {
        PROJECTILE_CARD,
        PROJECTILE_DAGGER,
        PROJECTILE_AIR,
        PROJECTILE_FIREBALL,
        PROJECTILE_SWORD,
        PROJECTILE_END
    };

    enum class TRIGGER_TYPE
    {
        TT_FADE_IN,
        TT_FADE_OUT,
        TT_FADE_AUTO,
        TT_INTERACTION,
        TT_END
    };

    enum class ITEM_TYPE
    {
        ITEM_KEY1,
        ITEM_KEY2,
        ITEM_MEMORY,
        ITEM_FORGIVEN,
        ITEM_SKILLPIECE,

        ITEM_FIELDITEM,


        ITEM_BADZOKER,
        ITEM_BEOMSEUNG,
        ITEM_FAKER,
        ITEM_UIN,
        ITEM_PLAYERMASTER,
        ITEM_YUBHIN,

        ITEM_HERB_1,
        ITEM_HERB_2,
        ITEM_HERB_3,
        ITEM_HERB_4,
        ITEM_HERB_5,
        ITEM_HERB_6,
        ITEM_HERB_7,

        ITEM_DEADBRANCH,
        ITEM_END
    };

    enum class EFFECT_TYPE
    {
        EFFECT_TYPE_PARTICLE,
        EFFECT_TYPE_SWORD,
        EFFECT_TYPE_MESH,
        EFFECT_TYPE_MESH_INSTANCING,
        EFFECT_TYPE_END
    };

    enum class EFFECT_NAME
    {
        //Mesh
        EFFECT_PLAYER_SWORD1,
        EFFECT_PLAYER_SWORD2,
        EFFECT_PLAYER_SWORD3,
        EFFECT_PLAYER_SWORD4_1,
        EFFECT_PLAYER_SWORD4_2,
        EFFECT_PLAYER_SWORD5,
        EFFECT_PLAYER_CLAW1,
        EFFECT_PLAYER_CLAW2,
        EFFECT_PLAYER_ATTACK5_DUST,
        EFFECT_VARG_ROAR,
        EFFECT_VARG_ROAR_BLINK,
        EFFECT_VARG_DEAD_BLINK,
        EFFECT_PLAYER_SCYTHE_1,
        EFFECT_PLAYER_SCYTHE_2,
        EFFECT_PLAYER_HALBERD,
        EFFECT_PLAYER_HEAL,
        EFFECT_MUTATION_BURST,
        EFFECT_MUTATION_IMPACT,
        EFFECT_MUTATION_SWEEP,
        EFFECT_JOKER_SHOCKWAVE,
        EFFECT_PLAYER_AXE,
        EFFECT_PLAYER_GREATSWORD,
        EFFECT_PLAYER_JAVELIN_DISTORTION,
        EFFECT_PLAYER_CHARGECLAW,
        EFFECT_BAT_AIR,
        EFFECT_VARG_INTRO,
        EFFECT_VARG_CATCH_EYE,
        EFFECT_PUNCH_DISTORTION,
        EFFECT_URD_STACK_SWORD,
        EFFECT_URD_STACK_SWORD_SURFACE,
        EFFECT_BAT_BLOOD_SUCK,
        EFFECT_BAT_CLAW_R,
        EFFECT_BAT_CLAW_L,
        EFFECT_WARNING,
        EFFECT_STACKSWORD_EXPLOSION_2,
        EFFECT_STACKSWORD_EXPLOSION_MAIN,
        EFFECT_STACKSWORD_EXPLOSION_SMALL,

        //Particle
        EFFECT_PARTICLE_DROP,
        EFFECT_PARTICLE_EXPLOSION,
        EFFECT_PARTICLE_SPARK,
        EFFECT_PARTICLE_SPARK_LEFT,
        EFFECT_PARTICLE_SPARK_RIGHT,
        EFFECT_PARTICLE_HOLDING,
        EFFECT_PARTICLE_BLOOD_1_HOLDING,
        EFFECT_PARTICLE_BLOOD_2_HOLDING,
        EFFECT_PARTICLE_BLOOD_3_HOLDING,
        EFFECT_PARTICLE_BLOOD_4_1_HOLDING,
        EFFECT_PARTICLE_BLOOD_4_2_HOLDING,
        EFFECT_PARTICLE_BLOOD_5_HOLDING,
        EFFECT_PARTICLE_PLAYERATTACK_5_DUST_EXPLOSION,
        EFFECT_PARTICLE_BLOOD_PLAYER_HIT_HOLDING,
        EFFECT_PARTICLE_SPARK_EXPLOSION,
        EFFECT_PARTICLE_VERTICAL_DUST,
        EFFECT_PARTICLE_LEFT_DUST_VARG,
        EFFECT_PARTICLE_RIGHT_DUST_VARG,
        EFFECT_PARTICLE_NARROW_DUST_VARG,
        EFFECT_PARTICLE_NARROW_SPARK_VARG,
        EFFECT_PARTICLE_HORIZON_DUST_VARG,
        EFFECT_PARTICLE_HORIZON_SPARK_VARG,
        EFFECT_PARTICLE_BLOOD_VARG_EXECUTION3,
        EFFECT_PARTICLE_BLOOD_VARG_EXECUTION1,
        EFFECT_PARTICLE_BLOOD_VARG_EXECUTION2,
        EFFECT_PARTICLE_SPARK_VARG_EXECUTION3,
        EFFECT_PARTICLE_SPARK_VARG_EXECUTION_1_2,
        EFFECT_PARTICLE_DUST_VARG_DEAD,
        EFFECT_PARTICLE_SPARK_VARG_DEAD,
        EFFECT_PARTICLE_DUSTDELAY_MUTATION_INTRO,
        EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_1,
        EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_2,
        EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_1,
        EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_2,
        EFFECT_PARTICLE_HURRICANE_HEAL,
        EFFECT_PARTICLE_HURRICANE_ITEM_GET_GREEN,
        EFFECT_PARTICLE_HURRICANE_ITEM_GET_RED,
        EFFECT_PARTICLE_HURRICANE_ITEM_GET_BLUE,
        EFFECT_PARTICLE_HURRICANE_ITEM_GET_YELLOW,
        EFFECT_PARTICLE_HURRICANE_ITEM_GET_WHITE,
        EFFECT_PARTICLE_EXPLOSION_MUTATION_BURST,
        EFFECT_PARTICLE_DUSTDELAY_MUTATION_BURST,
        EFFECT_PARTICLE_SPARK_MUTATION_IMPACT,
        EFFECT_PARTICLE_HURRICANE_MUTATION_FALLING_IMPACT,
        EFFECT_PARTICLE_HURRICANE_MUTATION_RISING_IMPACT,
        EFFECT_PARTICLE_EXPLOSION_MUTATION_STAB,
        EFFECT_PARTICLE_DUSTDELAY_MUTATION_STAB_DUST,
        EFFECT_PARTICLE_SPARK_MUTATION,
        EFFECT_PARTICLE_SPARK_HORIZON,
        EFFECT_PARTICLE_SPARK_LEFT_STAB,
        EFFECT_PARTICLE_SPARK_VERTICAL,
        EFFECT_PARTICLE_DUST_HORIZON,
        EFFECT_PARTICLE_DUST_NARROW,
        EFFECT_PARTICLE_DUST_JOKER_INTRO,
        EFFECT_PARTICLE_SPARK_JOKER_INTRO,
        EFFECT_PARTICLE_DUST_LEFT,
        EFFECT_PARTICLE_DUST_RIGHT,
        EFFECT_PARTICLE_SPARK_JOKER_SMASH,
        EFFECT_PARTICLE_DUST_SMASH,
        EFFECT_PARTICLE_DUST_SHOCKWAVE,
        EFFECT_PARTICLE_DUSTDELAY_SHOCKWAVE,
        EFFECT_PARTICLE_SCYTHE_WHEELATTACK,
        EFFECT_PARTICLE_SPARK_PLAYER_AXE,
        EFFECT_PARTICLE_DUST_PLAYER_AXE_HORIZON,
        EFFECT_PARTICLE_DUST_PLAYER_AXE_CROSS,
        EFFECT_PARTICLE_WORLD_PLAYER_CLAW,
        EFFECT_PARTICLE_WORLD_PLAYER_CANESWORD,
        EFFECT_PARTICLE_BURST_NARROW_VERTICAL,
        EFFECT_PARTICLE_BURST_PLAYER_CANESWORD,
        EFFECT_PARTICLE_DUST_NARROW_VERTICAL,
        EFFECT_PARTICLE_WORLD_GREATSWORD_START,
        EFFECT_PARTICLE_WORLD_GREATSWORD,
        EFFECT_PARTICLE_WORLD_JAVELIN,
        EFFECT_PARTICLE_WORLD_JAVELIN_START,
        EFFECT_PARTICLE_WORLD_JAVELIN_THROWING,
        EFFECT_PARTICLE_WORLD_CHARGECLAW,
        EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT,
        EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL,
        EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL_COMPLETE,
        EFFECT_PARTICLE_WORLD_CHARGECLAW_READY,
        EFFECT_PARTICLE_WORLD_CHARGECLAW_LOOP,
        EFFECT_PARTICLE_WORLD_PLAYER_FOOT_SMOKE_L,
        EFFECT_PARTICLE_WORLD_PLAYER_FOOT_SMOKE_R,
        EFFECT_PARTICLE_DUST_PLAYER_VARG_SPATTACK,
        EFFECT_PARTICLE_DUST_PLAYER_VARG_SPATTACK_HIT,
        EFFECT_PARTICLE_BLOOD_PLAYER_VARG_SPATTACK,
        EFFECT_PARTICLE_SPARK_PLAYER_VARG_SPATTACK,
        EFFECT_PARTICLE_DUST_SPORE,
        EFFECT_PARTICLE_WORLDGAS_SPORE,
        EFFECT_PARTICLE_JOKER_EXECUTION,
        EFFECT_PARTICLE_GRACE_EXECUTION_KICK,
        EFFECT_PARTICLE_GRACE_EXECUTION_BLOOD,
        EFFECT_PARTICLE_PUNCH_DUST_PUNCH_BIG,
        EFFECT_PARTICLE_PUNCH_DUST_PUNCH_SMALL,
        EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN,
        EFFECT_PARTICLE_PUNCH_SPARK_PUNCH_BIG,
        EFFECT_PARTICLE_PUNCH_EXECUTION,
        EFFECT_PARTICLE_HARMOR_EXECUTION_BLOOD_1,
        EFFECT_PARTICLE_HARMOR_EXECUTION_BLOOD_2,
        EFFECT_PARTICLE_HARMOR_EXECUTION_SPARK,
        EFFECT_PARTICLE_NORMAL_EXECUTION_KICK,
        EFFECT_PARTICLE_NORMAL_EXECUTION_STAB,
        EFFECT_PARTICLE_URD_STAB_1,
        EFFECT_PARTICLE_URD_STAB_2,
        EFFECT_PARTICLE_URD_STAB_2_WIDE,
        EFFECT_PARTICLE_URD_SWING_1,
        EFFECT_PARTICLE_URD_ATTACK_SMOKE,
        EFFECT_PARTICLE_URD_STEP_1,
        EFFECT_PARTICLE_URD_STEP_2,
        EFFECT_PARTICLE_URD_STEP_3,
        EFFECT_PARTICLE_URD_STEP_4,
        EFFECT_PARTICLE_URD_SP_START,
        EFFECT_PARTICLE_URD_SP_START_CONTINUE,
        EFFECT_PARTICLE_URD_SP_READY,
        EFFECT_PARTICLE_URD_SP_EXPLOSION,
        EFFECT_PARTICLE_URD_SKILL_SPARK,
        EFFECT_PARTICLE_URD_SP_DUST,
        EFFECT_PARTICLE_URD_SKILL_WORLD,
        EFFECT_PARTICLE_URD_SKILL_EXPLOSION_1,
        EFFECT_PARTICLE_URD_SKILL_EXPLOSION_2,
        EFFECT_PARTICLE_URD_SKILL_EXPLOSION_3,
        EFFECT_PARTICLE_URD_SKILL_EXPLOSION_4,
        EFFECT_PARTICLE_URD_CHARGE,
        EFFECT_PARTICLE_URD_CHARGE_FAST,
        EFFECT_PARTICLE_URD_INTRO_DUST,
        EFFECT_PARTICLE_URD_EXECUTION_SPARK,
        EFFECT_PARTICLE_ENVIRONMENT_LEAF,
        EFFECT_PARTICLE_ENVIRONMENT_DUST,
        EFFECT_PARTICLE_MAGICIAN_SWORD_DUST,
        EFFECT_PARTICLE_URD_EXECUTION_BLOOD_1,
        EFFECT_PARTICLE_URD_EXECUTION_BLOOD_2,
        EFFECT_PARTICLE_URD_STACKSWORD_EXPLOSION,
        EFFECT_PARTICLE_MAGICIAN_SWING_DUST,
        EFFECT_PARTICLE_MAGICIAN_FIRSTSWING_DUST,
        EFFECT_PARTICLE_MAGICIAN_TURNING_DUST,
        EFFECT_PARTICLE_MAGICIAN_CANESTAB,
        EFFECT_PARTICLE_MAGICIAN_KICK,
        EFFECT_PARTICLE_MAGICIAN_DISAPPEAR_START,
        EFFECT_PARTICLE_MAGICIAN_DISAPPEAR_MOVING,
        EFFECT_PARTICLE_MAGICIAN_PARRY,
        EFFECT_PARTICLE_MAGICIAN_BURST_1,
        EFFECT_PARTICLE_MAGICIAN_BURST_2,
        EFFECT_PARTICLE_MAGICIAN_STUN,
        EFFECT_PARTICLE_MAGICIAN_SPRINKLE,
        EFFECT_PARTICLE_MAGICIAN_SHOOT,
        EFFECT_PARTICLE_MAGICIAN_BURST_CHARGE,
        EFFECT_PARTICLE_MAGICIAN_SPATTACK_CHARGE,
        EFFECT_PARTICLE_MAGICIAN_SPATTACK_FAIL,
        EFFECT_PARTICLE_MAGICIAN_SPATTACK_SUCCESS,
        EFFECT_PARTICLE_MAGICIAN_SPATTACK_LASTCHARGE,
        EFFECT_PARTICLE_PLAYER_MAGICIAN_SPATTACK_LOOP,
        EFFECT_PARTICLE_PLAYER_MAGICIAN_SPATTACK_BLOOD_1,
        EFFECT_PARTICLE_PLAYER_MAGICIAN_SPATTACK_BLOOD_2,
        EFFECT_PARTICLE_PLAYER_MAGICIAN_SPATTACK_BLOOD_3,
        EFFECT_PARTICLE_PLAYER_MAGICIAN_SPATTACK_BLOOD_4,
        EFFECT_PARTICLE_PLAYER_MAGICIAN_SPATTACK_BLOOD_5,
        EFFECT_PARTICLE_PLAYER_MAGICIAN2_EXECUTION_BLOOD_1,
        EFFECT_PARTICLE_PLAYER_MAGICIAN2_EXECUTION_BLOOD_2_LEFT,
        EFFECT_PARTICLE_PLAYER_MAGICIAN2_EXECUTION_BLOOD_2_RIGHT,
        EFFECT_PARTICLE_MAGICIAN2_INTRO_FIRST,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_SWING_DUST,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_SWINGAROUND_DUST,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_SWINGREVERSE_DUST,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_SWINGBACK_DUST,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_SPECIALCHARGE_DUST,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_SPECIAL_SUCCESS,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_TENTACLE_LEAPATTACK,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_TENTACLE_FALLBACK,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE,
        EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE_BLUE,
        EFFECT_PARTICLE_RESEARCHER_WORLD_EXECUTION_1,
        EFFECT_PARTICLE_RESEARCHER_WORLD_EXECUTION_2,
        EFFECT_PARTICLE_RESEARCHER_EXECUTION_3,
        EFFECT_PARTICLE_STACKSWORD_EXPLOSION,
        EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_1,
        EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_2,
        EFFECT_PARTICLE_URD_WORLD_SWING_DUST,
        EFFECT_PARTICLE_URD_WORLD_SWINGRIGHT_DUST,
        EFFECT_PARTICLE_URD_WORLD_SWINGLEFT_DUST,
        EFFECT_PARTICLE_URD_WORLD_SWING_CHARGE,
        EFFECT_PARTICLE_RESEARCHER_WORLD_SPECIAL,
        EFFECT_PARTICLE_CHAIR,
        EFFECT_PARTICLE_PLAYER_MANA,
        EFFECT_PARTICLE_PLAYER_JAVELIN_HIT,

        EFFECT_PARTICLE_WARNING,
        EFFECT_PARTICLE_CARD_DAMAGE,

        //강범승 추가Effect
        EFFECT_PARTICLE_MAGICIAN_EXECUTION_BLOOD_1,
		EFFECT_PARTICLE_MAGICIAN_EXECUTION_BLOOD_2,
		EFFECT_PARTICLE_MAGICIAN_EXECUTION_DUST_2,
		EFFECT_PARTICLE_BAT_SPECIAL_ATTACK,

        //임상혁 추가 Effect
        EFFECT_PARTICLE_WORLD_RESEARCHER_PROJECTILE_SMOKE,
        EFFECT_PARTICLE_WORLD_DAGGER_PROJECTILE_SMOKE,
        EFFECT_PARTICLE_BAT_EXECUTION_BLOOD_1,
        EFFECT_PARTICLE_BAT_EXECUTION_BLOOD_2,
        EFFECT_PARTICLE_BAT_EXECUTION_BLOOD_3,
        EFFECT_PARTICLE_WORLD_BAT_RED_DUST_1,
        EFFECT_PARTICLE_WORLD_BAT_SALIVIA,
        //한유인 추가 Effect
        EFFECT_PARTCLE_GHOSEMY_DUST,
        EFFECT_PARTICLE_BLOOD_HAND_LEFT_PYUNGTA,
        EFFECT_PARTICLE_BLOOD_HAND_RIGHT_PYUNGTA,
        EFFECT_PARTICLE_BLOOD_SESU,
        EFFECT_PARTICLE_SURROND,
        EFFECT_PARTICLE_BACKSLIP,
        EFFECT_PARTICLE_CHESTATTACK_LEFT,
        EFFECT_PARTICLE_CHESTATTACK_RIGHT,

        //Sword
        EFFECT_SWORD_VARG,
        EFFECT_SWORD_CLAW_1,
        EFFECT_SWORD_CLAW_2,
        EFFECT_SWORD_MUTATION,
        EFFECT_SWORD_JOKER,
        EFFECT_SWORD_HARMOR,
        EFFECT_SWORD_PLAYER_EYE,
        EFFECT_SWORD_VARG_EYE,
        EFFECT_SWORD_MAGICIAN_SWORD,
        EFFECT_SWORD_MAGICIAN_CANE,
        EFFECT_SWORD_PROJECTILE,

        //Mesh Instancing
        EFFECT_MESH_INSTANCING_ROCK,

        // 한유인 추가 Instancing Effect
        EFFECT_MESH_INSTANCING_BAT_HANDATTACK_LEFT,
        EFFECT_MESH_INSTANCING_BAT_HANDATTACK_RIGHT,
        EFFECT_MESH_INSTANCING_BAT_HANDATTACK_JUMP_SMASH_CHEST_ATTACK,
        EFFECT_MESH_INSTANCING_BAT_HANDATTACK_JUMP_SMASH_LEFT_HAND_ATTACK,
        EFFECT_MESH_INSTANCING_BAT_SCRACTCHING_LEFT_HAND,
        EFFECT_MESH_INSTANCING_BAT_SCRACTCHING_RIGHT_HAND,
        EFFECT_MESH_INSTANCING_JOKER_JUMP_ATTACK,
        EFFECT_MESH_INSTANCING_JOKER_RUN_ATTACK,
        EFFECT_MESH_INSTANCING_JOKER_TURN_ATTACK,
        EFFECT_MESH_INSTANCING_JOKER_SHOCKWAVE,
        EFFECT_MESH_INSTANCING_PUNCHMAN_ATTACK,
        EFFECT_MESH_INSTANCING_MUTATION_COMBO_B,
        EFFECT_MESH_INSTANCING_MUTATION_COMBO_J,
        EFFECT_MESH_INSTANCING_MUTATION_COMBO_F,
        EFFECT_MESH_INSTANCING_MUTATION_SP_ATTACK,
        EFFECT_MESH_INSTANCING_URD_NORMAL_DAGGER_SKILL,
        EFFECT_MESH_INSTANCING_URD_SPECIAL_SKILL,

        EFFECT_NAME_END
    };

}
#endif // Engine_Enum_h__
