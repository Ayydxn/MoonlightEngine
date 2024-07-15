#pragma once

#include "Logging/Logging.h"

#define MOONLIGHT_DEBUGBREAK() *reinterpret_cast<int32*>(3) = 13

#ifdef MOONLIGHT_BUILD_DEBUG
    #define MOONLIGHT_ENABLE_ASSERTIONS
#endif

#define MOONLIGHT_ENABLE_VERIFIES

/* The difference between the macros suffixed with a 'f' and the ones that aren't is the ones which are suffixed support logging a message,
*  while the ones that aren't suffixed don't support logging a message.
*
*  This takes after what is present in Unreal Engine's API for assertions/verifies.
*/

#ifdef MOONLIGHT_ENABLE_ASSERTIONS
    #define ENGINE_ASSERT_INTERNAL()                           CLogging::LogAssertMessage(ELogType::Engine, "Assertion Failed", "")
    #define APPLICATION_ASSERT_INTERNAL()                      CLogging::LogAssertMessage(ELogType::Application, "Assertion Failed", "")

    #define ENGINE_ASSERT_MESSAGE_INTERNAL(Message, ...)       CLogging::LogAssertMessage(ELogType::Engine, "Assertion Failed", Message, ##__VA_ARGS__)
    #define APPLICATION_ASSERT_MESSAGE_INTERNAL(Message, ...)  CLogging::LogAssertMessage(ELogType::Application, "Assertion Failed", Message, ##__VA_ARGS__)

    #define assertEngine(Condition) { if(!(Condition)) { ENGINE_ASSERT_INTERNAL(); MOONLIGHT_DEBUGBREAK(); } }
    #define assertApplication(Condition) { if(!(Condition)) { APPLICATION_ASSERT_INTERNAL(); MOONLIGHT_DEBUGBREAK(); } }

    #define assertEnginef(Condition, Message, ...) { if(!(Condition)) { ENGINE_ASSERT_MESSAGE_INTERNAL(Message, ##__VA_ARGS__); MOONLIGHT_DEBUGBREAK(); } }
    #define assertApplicationf(Condition, Message, ...) { if(!(Condition)) { APPLICATION_ASSERT_MESSAGE_INTERNAL(Message, ##__VA_ARGS__); MOONLIGHT_DEBUGBREAK(); } }
#else
    #define assertEngine(Condition, ...);
    #define assertApplication(Condition, ...);

    #define assertEnginef(Condition, Message, ...);
    #define assertApplicationf(Condition, Message, ...);
#endif

#ifdef MOONLIGHT_ENABLE_VERIFIES
    #define ENGINE_VERIFY_INTERNAL()               CLogging::LogAssertMessage(ELogType::Engine, "Verify Failed", "")
    #define APPLICATION_VERIFY_INTERNAL()          CLogging::LogAssertMessage(ELogType::Application, "Verify Failed", "")

    #define ENGINE_VERIFY_MESSAGE_INTERNAL(Message, ...)       CLogging::LogAssertMessage(ELogType::Engine, "Verify Failed", Message, ##__VA_ARGS__)
    #define APPLICATION_VERIFY_MESSAGE_INTERNAL(Message, ...)  CLogging::LogAssertMessage(ELogType::Application, "Verify Failed", Message, ##__VA_ARGS__)

    #define verifyEngine(Condition) { if(!(Condition)) { ENGINE_VERIFY_INTERNAL(); MOONLIGHT_DEBUGBREAK(); } }
    #define verifyApplication(Condition) { if(!(Condition)) { APPLICATION_VERIFY_INTERNAL(); MOONLIGHT_DEBUGBREAK(); } }

    #define verifyEnginef(Condition, Message, ...) { if(!(Condition)) { ENGINE_VERIFY_MESSAGE_INTERNAL(Message, ##__VA_ARGS__); MOONLIGHT_DEBUGBREAK(); } }
    #define verifyApplicationf(Condition, Message, ...) { if(!(Condition)) { APPLICATION_VERIFY_MESSAGE_INTERNAL(Message, ##__VA_ARGS__); MOONLIGHT_DEBUGBREAK(); } }
#else
    #define verifyEngine(Condition, ...);
    #define verifyApplication(Condition, ...);

    #define verifyEnginef(Condition, Message, ...);
    #define verifyApplicationf(Condition, Message, ...);
#endif