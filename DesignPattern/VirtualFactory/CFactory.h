#ifndef VIRTUAL_FACTORY_H
#define VIRTUAL_FACTORY_H

#include <stdbool.h>
#include <stdint.h>

#define ERR_SUCCESS 0U
#define ERR_PARAM_INVALID 1U
#define INSTANCE_INVALID 0xFFFFU

typedef uint32_t (*InstantiationHandle)(void* instance, void* para);

#define INSTANTIATION_FACTORY_DEF(VirFactoryName, virFactoryTypeSpec)                            \
    static InstantiationHandle g_##VirFactoryName##InstantiationHandles[virFactoryTypeSpec];     \
                                                                                                 \
    static inline void Init##VirFactoryName##InstantiationFactory(void) {                        \
        for (uint32_t i = 0U; i < (uint32_t)(virFactoryTypeSpec); i++) {                         \
            g_##VirFactoryName##InstantiationHandles[i] = NULL;                                  \
        }                                                                                        \
    }                                                                                            \
                                                                                                 \
    static inline uint32_t Reg##VirFactoryName##InstantiationHandle(                             \
        uint32_t instanceType, InstantiationHandle pfnNewInstance) {                             \
        if (instanceType >= (uint32_t)(virFactoryTypeSpec)) {                                    \
            return ERR_PARAM_INVALID;                                                            \
        }                                                                                        \
        g_##VirFactoryName##InstantiationHandles[instanceType] = pfnNewInstance;                 \
        return ERR_SUCCESS;                                                                      \
    }                                                                                            \
                                                                                                 \
    static inline uint32_t Instantiation##VirFactoryName##Instance(uint32_t instanceType,        \
                                                                   void* instance, void* para) { \
        if ((instanceType >= (uint32_t)(virFactoryTypeSpec)) ||                                  \
            (g_##VirFactoryName##InstantiationHandles[instanceType] == NULL)) {                  \
            return ERR_PARAM_INVALID;                                                            \
        }                                                                                        \
        return g_##VirFactoryName##InstantiationHandles[instanceType](instance, para);           \
    }

#define INIT_VIRT_FACTORY(VirFactoryName) Init##VirFactoryName##InstantiationFactory()
#define REG_INSTANTIATION_HANDLE(VirFactoryName, instanceType, pfnNewInstance) \
    Reg##VirFactoryName##InstantiationHandle((instanceType), (pfnNewInstance))
#define INSTANTIATION_INSTANCE(VirFactoryName, instanceType, instance, para) \
    Instantiation##VirFactoryName##Instance((instanceType), (instance), (para))

static inline uint16_t InstancelntToUint16(int32_t index) { return (uint16_t)index; }

#define INSTANCE_DEF(InstanceType, spec)                                                    \
    typedef struct {                                                                        \
        uint16_t instanceNum;                                                               \
        InstanceType instance[(spec)];                                                      \
    } InstanceType##Factory;                                                                \
                                                                                            \
    static InstanceType##Factory g_##InstanceType##Factory = {.instanceNum = 0U};           \
                                                                                            \
    static inline uint16_t New##InstanceType##Id(void) {                                    \
        if (g_##InstanceType##Factory.instanceNum >= (uint16_t)(spec)) {                    \
            return INSTANCE_INVALID;                                                        \
        }                                                                                   \
        return g_##InstanceType##Factory.instanceNum++;                                     \
    }                                                                                       \
                                                                                            \
    static inline void Free##InstanceType##Id(uint16_t id) {                                \
        if (((uint16_t)(id + 1U)) == g_##InstanceType##Factory.instanceNum &&               \
            (g_##InstanceType##Factory.instanceNum > 0U)) {                                 \
            g_##InstanceType##Factory.instanceNum--;                                        \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    static inline InstanceType* Get##InstanceType##Addr(uint16_t id) {                      \
        return (id < g_##InstanceType##Factory.instanceNum)                                 \
                   ? &g_##InstanceType##Factory.instance[id]                                \
                   : NULL;                                                                  \
    }                                                                                       \
                                                                                            \
    static inline uint16_t Get##InstanceType##Id(InstanceType* instance) {                  \
        if ((instance < g_##InstanceType##Factory.instance) ||                              \
            (instance >=                                                                    \
             &g_##InstanceType##Factory.instance[g_##InstanceType##Factory.instanceNum])) { \
            return INSTANCE_INVALID;                                                        \
        }                                                                                   \
        return (uint16_t)(instance - g_##InstanceType##Factory.instance);                   \
    }                                                                                       \
                                                                                            \
    static inline uint16_t Get##InstanceType##ActiveNum(void) {                             \
        return g_##InstanceType##Factory.instanceNum;                                       \
    }                                                                                       \
                                                                                            \
    static inline InstanceType* New##InstanceType(void) {                                   \
        uint16_t inst = New##InstanceType##Id();                                            \
        return Get##InstanceType##Addr(inst);                                               \
    }                                                                                       \
                                                                                            \
    static inline void Free##InstanceType##Instance(InstanceType* instance) {               \
        uint16_t inst = InstancelntToUint16(instance - g_##InstanceType##Factory.instance); \
        Free##InstanceType##Id(inst);                                                       \
    }                                                                                       \
                                                                                            \
    typedef bool (*check##InstanceType##CfgFun)(InstanceType * base, const void* key);      \
    static inline InstanceType* Get##InstanceType##AddrByCfg(                               \
        const void* keyCfg, check##InstanceType##CfgFun checkFun) {                         \
        for (uint16_t inst = 0U; inst < g_##InstanceType##Factory.instanceNum; inst++) {    \
            InstanceType* base = &g_##InstanceType##Factory.instance[inst];                 \
            if (checkFun(base, keyCfg)) {                                                   \
                return base;                                                                \
            }                                                                               \
        }                                                                                   \
        return NULL;                                                                        \
    }

#define INSTANCE_GET_BY_CFG(InstanceType, key, fun) Get##InstanceType##AddrByCfg(key, fun)
#define INSTANCE_NEW_ID(InstanceType) New##InstanceType##Id()
#define INSTANCE_ACTIVE_NUM(InstanceType) Get##InstanceType##ActiveNum()
#define INSTANCE_FREE_ID(InstanceType, id) Free##InstanceType##Id((id))
#define INSTANCE_GET(InstanceType, id) Get##InstanceType##Addr(id)
#define INSTANCE_GET_ID(InstanceType, instance) Get##InstanceType##Id(instance)
#define INSTANCE_NEW(InstanceType) New##InstanceType()
#define INSTANCE_FREE(InstanceType, instance) Free##InstanceType##Instance(instance)

#endif  // VIRTUAL_FACTORY_H
