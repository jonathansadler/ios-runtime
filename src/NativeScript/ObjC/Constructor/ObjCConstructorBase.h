//
//  ObjCConstructorBase.h
//  NativeScript
//
//  Created by Yavor Georgiev on 17.07.14.
//  Copyright (c) 2014 г. Telerik. All rights reserved.
//

#ifndef __NativeScript__ObjCConstructorBase__
#define __NativeScript__ObjCConstructorBase__

#include "ConstructorKey.h"
#include "FFIType.h"
#include <functional>

namespace Metadata {
struct InterfaceMeta;
}

namespace NativeScript {
class ObjCConstructorWrapper;
class ObjCPrototype;

class ObjCConstructorBase : public JSC::InternalFunction {
public:
    typedef JSC::InternalFunction Base;

    static const unsigned StructureFlags;

    DECLARE_INFO;

    const Metadata::KnownUnknownClassPair& klasses() const {
        return this->_key.klasses;
    }

    const Metadata::ProtocolMetas& additionalProtocols() const {
        return this->_key.additionalProtocols;
    }

    JSC::Structure* instancesStructure() const {
        return this->_instancesStructure.get();
    }

    const FFITypeMethodTable& ffiTypeMethodTable() {
        return this->_ffiTypeMethodTable;
    }

    DECLARE_CLASSNAME()

    const WTF::Vector<JSC::WriteBarrier<ObjCConstructorWrapper>>& initializers(JSC::VM&, GlobalObject*);

    const Metadata::InterfaceMeta* metadata();

    ObjCPrototype* getObjCPrototype() const;

protected:
    ObjCConstructorBase(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure, createObjCClass, constructObjCClass) {
    }

    static void destroy(JSC::JSCell* cell) {
        static_cast<ObjCConstructorBase*>(cell)->~ObjCConstructorBase();
    }

    void finishCreation(JSC::VM&, JSC::JSGlobalObject*, JSC::JSObject* prototype, const ConstructorKey& key);

    static void visitChildren(JSC::JSCell*, JSC::SlotVisitor&);

    static bool getOwnPropertySlot(JSC::JSObject*, JSC::ExecState*, JSC::PropertyName, JSC::PropertySlot&);

    const Metadata::InterfaceMeta* _metadata;

    static JSC::EncodedJSValue JSC_HOST_CALL constructObjCClass(JSC::ExecState* execState);

    static JSC::EncodedJSValue JSC_HOST_CALL createObjCClass(JSC::ExecState* execState);

private:
    static JSC::JSValue read(JSC::ExecState*, void const*, JSC::JSCell*);

    static void write(JSC::ExecState*, const JSC::JSValue&, void*, JSC::JSCell*);

    /// Used when checking if a specific JSValue can be marshalled to native as
    /// this constructor function's underlying type.
    static bool canConvert(JSC::ExecState*, const JSC::JSValue&, JSC::JSCell*);

    static const char* encode(JSC::VM&, JSC::JSCell*);

    ConstructorKey _key;

    JSC::WriteBarrier<JSC::JSObject> _prototype;

    WTF::Vector<JSC::WriteBarrier<ObjCConstructorWrapper>> _initializers;

    JSC::WriteBarrier<JSC::Structure> _instancesStructure;

    FFITypeMethodTable _ffiTypeMethodTable;
};
} // namespace NativeScript

#endif /* defined(__NativeScript__ObjCConstructorBase__) */
