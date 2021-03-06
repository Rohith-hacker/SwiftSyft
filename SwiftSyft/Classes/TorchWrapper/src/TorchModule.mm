#import "TorchModule.h"
#import <LibTorch/LibTorch.h>
#import "TorchIValuePrivate.h"

@implementation TorchModule {
  torch::jit::script::Module _impl;
}

+ (TorchModule*)loadTorchscriptModel:(NSString*)modelPath {
  if (modelPath.length == 0) {
    return nil;
  }
  try {
    auto qengines = at::globalContext().supportedQEngines();
    if (std::find(qengines.begin(), qengines.end(), at::QEngine::QNNPACK) != qengines.end()) {
      at::globalContext().setQEngine(at::QEngine::QNNPACK);
    }
    TorchModule* module = [[TorchModule alloc] init];
    module->_impl = torch::jit::load([modelPath cStringUsingEncoding:NSASCIIStringEncoding]);
    module->_impl.eval();
    return module;
  } catch (const std::exception& exception) {
    NSLog(@"%s", exception.what());
  }
  return nil;
}

- (TorchIValue*)forward:(NSArray<TorchIValue*>*)values {
  std::vector<at::IValue> inputs;
  for (TorchIValue* value in values) {
    at::IValue atValue = value.toIValue;
    inputs.push_back(atValue);
  }
  try {
    torch::autograd::AutoGradMode guard(false);
    at::AutoNonVariableTypeMode non_var_type_mode(true);
    auto result = _impl.forward(inputs);
    return [TorchIValue newWithIValue:result];
  } catch (const std::exception& exception) {
    NSLog(@"%s", exception.what());
  }
  return nil;
}

- (TorchIValue*)run_method:(NSString*)methodName withInputs:(NSArray<TorchIValue*>*)values {
  if (methodName.length == 0) {
    return nil;
  }
  std::vector<at::IValue> inputs;
  for (TorchIValue* value in values) {
    inputs.push_back(value.toIValue);
  }
  try {
    if (auto method = _impl.find_method(
            std::string([methodName cStringUsingEncoding:NSASCIIStringEncoding]))) {
      torch::autograd::AutoGradMode guard(false);
      at::AutoNonVariableTypeMode non_var_type_mode(true);
      auto result = (*method)(std::move(inputs));
      return [TorchIValue newWithIValue:result];
    }
  } catch (const std::exception& exception) {
    NSLog(@"%s", exception.what());
  }
  return nil;
}

@end
