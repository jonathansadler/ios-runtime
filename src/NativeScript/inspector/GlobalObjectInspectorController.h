/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GlobalObjectInspectorController_h
#define GlobalObjectInspectorController_h

#include "InspectorAgentRegistry.h"
#include "InspectorEnvironment.h"
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/text/WTFString.h>

#if ENABLE(INSPECTOR_ALTERNATE_DISPATCHERS)
#include "AugmentableInspectorController.h"
#endif

namespace WTF {
    class Stopwatch;
}


namespace JSC {
    class ConsoleClient;
    class Exception;
    class ExecState;
    class JSGlobalObject;
    class JSValue;
}

namespace Inspector {
    
    class BackendDispatcher;
    class FrontendChannel;
    class InjectedScriptManager;
    class InspectorAgent;
    class InspectorConsoleAgent;
    class InspectorDebuggerAgent;
    class InspectorTimelineAgent;
    class JSGlobalObjectConsoleClient;
    class ScriptCallStack;
    
    class GlobalObjectInspectorController final
    : public InspectorEnvironment
#if ENABLE(INSPECTOR_ALTERNATE_DISPATCHERS)
    , public AugmentableInspectorController
#endif
    {
        WTF_MAKE_NONCOPYABLE(GlobalObjectInspectorController);
        WTF_MAKE_FAST_ALLOCATED;
    public:
        GlobalObjectInspectorController(JSC::JSGlobalObject&);
        ~GlobalObjectInspectorController();
        
        void connectFrontend(FrontendChannel*, bool isAutomaticInspection);
        void disconnectFrontend(DisconnectReason);
        void dispatchMessageFromFrontend(const String&);
        
        void globalObjectDestroyed();
        
        bool includesNativeCallStackWhenReportingExceptions() const { return m_includeNativeCallStackWithExceptions; }
        void setIncludesNativeCallStackWhenReportingExceptions(bool includesNativeCallStack) { m_includeNativeCallStackWithExceptions = includesNativeCallStack; }
        
        void pause();
        void reportAPIException(JSC::ExecState*, JSC::Exception*);
        
        JSC::ConsoleClient* consoleClient() const;
        InspectorTimelineAgent* timelineAgent() const;
        
        virtual bool developerExtrasEnabled() const override;
        virtual bool canAccessInspectedScriptState(JSC::ExecState*) const override { return true; }
        virtual InspectorFunctionCallHandler functionCallHandler() const override;
        virtual InspectorEvaluateHandler evaluateHandler() const override;
        virtual void willCallInjectedScriptFunction(JSC::ExecState*, const String&, int) override { }
        virtual void didCallInjectedScriptFunction(JSC::ExecState*) override { }
        virtual void frontendInitialized() override;
        virtual Ref<WTF::Stopwatch> executionStopwatch() override;
        
#if ENABLE(INSPECTOR_ALTERNATE_DISPATCHERS)
        virtual AugmentableInspectorControllerClient* augmentableInspectorControllerClient() const override { return m_augmentingClient; }
        virtual void setAugmentableInspectorControllerClient(AugmentableInspectorControllerClient* client) override { m_augmentingClient = client; }
        
        virtual FrontendChannel* frontendChannel() const override { return m_frontendChannel; }
        virtual void appendExtraAgent(std::unique_ptr<InspectorAgentBase>) override;
#endif
        
    private:
        void appendAPIBacktrace(ScriptCallStack* callStack);
        
        JSC::JSGlobalObject& m_globalObject;
        std::unique_ptr<InjectedScriptManager> m_injectedScriptManager;
        std::unique_ptr<JSGlobalObjectConsoleClient> m_consoleClient;
        InspectorAgent* m_inspectorAgent;
        InspectorConsoleAgent* m_consoleAgent;
        InspectorDebuggerAgent* m_debuggerAgent;
        InspectorTimelineAgent* m_timelineAgent;
        AgentRegistry m_agents;
        FrontendChannel* m_frontendChannel;
        RefPtr<BackendDispatcher> m_backendDispatcher;
        Ref<WTF::Stopwatch> m_executionStopwatch;
        bool m_includeNativeCallStackWithExceptions;
        bool m_isAutomaticInspection;
        
#if ENABLE(INSPECTOR_ALTERNATE_DISPATCHERS)
        AugmentableInspectorControllerClient* m_augmentingClient;
#endif
    };
    
} // namespace Inspector

#endif // !defined(GlobalObjectInspectorController_h)
