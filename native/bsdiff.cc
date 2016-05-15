#include <node.h>
#include <node_buffer.h>
#include "../libminibsdiff/bsdiff.h"
#include "../libminibsdiff/bspatch.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Exception;
using v8::MaybeLocal;

using namespace node;

namespace bsdiff_addon {

void diff(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "Both old and new String or Buffer is required"
        )));
    return;
  }

  if ((!args[0]->IsString() && !Buffer::HasInstance(args[0]))
  	|| (!args[1]->IsString() && !Buffer::HasInstance(args[1]))) {
  	isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "Both arguments have to be String or Buffer"
        )));
    return;
  }

  MaybeLocal<Object> oldDamnBuf, newDamnBuf, patchDamnBuf;

  if (args[0]->IsString()) {
  	oldDamnBuf = Buffer::New(isolate, args[0]->ToString(), BINARY);
  } else {
  	oldDamnBuf = args[0]->ToObject();
  }

  if (args[1]->IsString()) {
		newDamnBuf = Buffer::New(isolate, args[1]->ToString(), BINARY);
  } else {
  	newDamnBuf = args[1]->ToObject();
  }

  Local<Object> oldBuf = oldDamnBuf.ToLocalChecked();
  Local<Object> newBuf = newDamnBuf.ToLocalChecked();
  off_t patch_size = bsdiff_patchsize_max(
  	Buffer::Length(oldBuf), Buffer::Length(newBuf)
  );

  Local<Object> patchBuf = Buffer::New(isolate, patch_size).ToLocalChecked();
  int patch_bytes = bsdiff((u_char*) Buffer::Data(oldBuf), Buffer::Length(oldBuf),
  	(u_char*)Buffer::Data(newBuf), Buffer::Length(newBuf),
  	(u_char*)Buffer::Data(patchBuf), Buffer::Length(patchBuf));
  if (patch_bytes < 0) {
  	isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "Maybe no enough memory"
        )));
    return;
  }

  args.GetReturnValue().Set(
  	Buffer::New(isolate, Buffer::Data(patchBuf), patch_bytes).ToLocalChecked()
  );
}

void patch(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "Both old and patch String or Buffer is required"
        )));
    return;
  }

  if ((!args[0]->IsString() && !Buffer::HasInstance(args[0]))
  	|| (!args[1]->IsString() && !Buffer::HasInstance(args[1]))) {
  	isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "Both arguments have to be String or Buffer"
        )));
    return;
  }

  MaybeLocal<Object> oldDamnBuf, patchDamnBuf;

  if (args[0]->IsString()) {
  	oldDamnBuf = Buffer::New(isolate, args[0]->ToString(), BINARY);
  } else {
  	oldDamnBuf = args[0]->ToObject();
  }

  if (args[1]->IsString()) {
		patchDamnBuf = Buffer::New(isolate, args[1]->ToString(), BINARY);
  } else {
  	patchDamnBuf = args[1]->ToObject();
  }

  Local<Object> oldBuf = oldDamnBuf.ToLocalChecked();
  Local<Object> patchBuf = patchDamnBuf.ToLocalChecked();
  off_t new_size = bspatch_newsize(
  	(u_char*)Buffer::Data(patchBuf), Buffer::Length(patchBuf)
  );

  if (new_size < 0) {
  	isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "You patch is corrupt"
        )));
    return;
  }

  Local<Object> newBuf = Buffer::New(isolate, new_size).ToLocalChecked();
  int new_bytes = bspatch((u_char*) Buffer::Data(oldBuf), Buffer::Length(oldBuf),
  	(u_char*)Buffer::Data(patchBuf), Buffer::Length(patchBuf),
  	(u_char*)Buffer::Data(newBuf), Buffer::Length(newBuf));
  switch (new_bytes) {
  	case -1:
  		isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "Maybe no enough memory"
        )));
  		return;
  	case -2:
  		isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "The patch header is invalid."
        )));
    	return;
  	case -3:
  		isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(
        	isolate, "The patch is corrupt"
        )));
    	return;
  }

  args.GetReturnValue().Set(newBuf);
}

void init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "diff", diff);
	NODE_SET_METHOD(exports, "patch", patch);
}

NODE_MODULE(bsdiff_addon, init)

}