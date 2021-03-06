#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, TorchTensorType) {
  TorchTensorTypeByte,       // 8bit unsigned integer
  TorchTensorTypeChar,       // 8bit signed integer
  TorchTensorTypeInt,        // 32bit signed integer
  TorchTensorTypeLong,       // 64bit signed integer
  TorchTensorTypeFloat,      // 32bit single precision floating point
  TorchTensorTypeDouble,      // 64bit double precision floating point
  TorchTensorTypeBool,      // Boolean
  TorchTensorTypeUndefined,  // Undefined tensor type. This indicates an error with the model
};
/**
 An input or output tensor model
 */
@interface TorchTensor : NSObject <NSCopying>
/**
 Data type of the tensor
 */
@property(nonatomic, readonly) TorchTensorType dtype;
/**
//The size of the tensor. The returned value is a array of integer
 */
@property(nonatomic, readonly) NSArray<NSNumber*>* sizes;
/**
 /The number of dimensions of the tensor.
 */
@property(nonatomic, readonly) int64_t dim;
/**
 The total number of elements in the input tensor.
 */
@property(nonatomic, readonly) int64_t numel;
/**
 The raw buffer of tensor
 */
@property(nonatomic, readonly) void* data;

/**
    Holds a strong reference to the buffer of the tensor so it doesn't get deallocated during the liftime of the tensor
 */
@property (strong, nonatomic) NSValue *pointerValue;

@property (nonatomic, copy) void (^deinitBlock)(void);

/**
 Creat a tensor object with data type, shape and a raw pointer to a data buffer.

 @param type Data type of the tensor
 @param size Size of the tensor
 @param data A raw pointer to a data buffer
 @return  A tensor object
 */
+ (nullable TorchTensor*)newWithData:(void*)data
                                Size:(NSArray<NSNumber*>*)size
                                Type:(TorchTensorType)type;

# pragma mark - TorchTensor operations

- (BOOL)isEqual:(nullable id)object;
- (nullable TorchTensor *)sub:(TorchTensor *)other error:(NSError **)error;
- (nullable TorchTensor *)add:(TorchTensor *)other error:(NSError **)error;
- (nullable TorchTensor *)mul:(TorchTensor *)other error:(NSError **)error;
- (nullable TorchTensor *)div:(TorchTensor *)other error:(NSError **)error;

- (void)print;
- (TorchTensor *)reshape:(NSArray<NSNumber *> *)shape;

// Gets scalar value out of tensor. Only works for float tensors
- (float)item;

- (NSArray<NSNumber *> *)toArray;
+ (TorchTensor *)cat:(NSArray<TorchTensor *> *)tensor;

@end

@interface TorchTensor (ObjectSubscripting)
/**
 This allows the tensor obejct to do subscripting. For example, let's say the shape of the current
 tensor is `1x10`, then tensor[0] will give you a one dimentional array of 10 tensors.
 NOTE: Subscripting could be very slow. Internally it creates a new tensor every time.
 A fast way to get the data from tensor is through `self.data`.

 @param idx index
 @return A new tensor object with the given index
 */
- (nullable TorchTensor*)objectAtIndexedSubscript:(NSUInteger)idx;

@end

NS_ASSUME_NONNULL_END
