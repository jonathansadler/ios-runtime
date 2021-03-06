//
//  PointerInstance.h
//  NativeScript
//
//  Created by Jason Zhekov on 8/19/14.
//  Copyright (c) 2014 Telerik. All rights reserved.
//

#ifndef __NativeScript__PointerInstance__
#define __NativeScript__PointerInstance__

namespace NativeScript {

/**
 * \brief The function is executed when a type object is called as JS function.
 */
JSC::EncodedJSValue JSC_HOST_CALL readFromPointer(JSC::ExecState* execState);

class PointerInstance : public JSC::JSDestructibleObject {
public:
    typedef JSC::JSDestructibleObject Base;

    DECLARE_INFO;

    static JSC::Strong<PointerInstance> create(JSC::ExecState* execState, JSC::Structure* structure, void* value = nullptr) {
        JSC::VM& vm = execState->vm();
        JSC::Strong<PointerInstance> object(vm, new (NotNull, JSC::allocateCell<PointerInstance>(vm.heap)) PointerInstance(vm, structure));
        object->finishCreation(execState, value);
        return object;
    }

    static JSC::Structure* createStructure(JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(globalObject->vm(), globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    void* data() const {
        return this->_data;
    }

    void reset() {
        this->_data = nullptr;
    }

    bool isAdopted() const {
        return this->_isAdopted;
    }

    void setAdopted(bool value) {
        this->_isAdopted = value;
    }

private:
    PointerInstance(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    ~PointerInstance();

    static void destroy(JSC::JSCell* cell) {
        static_cast<PointerInstance*>(cell)->~PointerInstance();
    }

    void finishCreation(JSC::ExecState*, void*);

    void* _data;

    bool _isAdopted = false;
};
} // namespace NativeScript

#endif /* defined(__NativeScript__PointerInstance__) */
