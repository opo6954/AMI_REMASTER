#ifndef VGGFACE_NETWORK_H
#define VGGFACE_NETWORK_H

#ifdef  VGGFACE_DLL_EXPORTS 
	#define DLLEXPORT __declspec(dllexport)

#else
	#define DLLEXPORT __declspec(dllimport)
#endif

#define DEVICE_GPU vl::DeviceType::VLDT_GPU
#define DEVICE_CPU vl::DeviceType::VLDT_CPU

#include <vector>
#include <cuda/cuda.h>
#include "matconvnet/bits/data.hpp"

typedef vl::DeviceType VGG_FACE_Device;

struct layer;
typedef struct layer layer;


class DLLEXPORT TensorShape
{
public:
	TensorShape (const vl::TensorShape& x);
	TensorShape& operator= (const vl::TensorShape& x);

	size_t getDimension(size_t num) const;
	size_t const * getDimensions() const ;
	size_t getNumDimensions() const ;
	size_t getHeight() const ;
	size_t getWidth() const ;
	size_t getDepth() const ;
	size_t getSize() const ;

	size_t getNumElements() const ;

protected:
	size_t dimensions [VL_TENSOR_SHAPE_MAX_NUM_DIMENSIONS] ;
	size_t numDimensions ;
};

class DLLEXPORT vgg_face_network {
public:
	vgg_face_network(std::string vggface_path, std::string meta_path, VGG_FACE_Device device_type);
	vgg_face_network(const vgg_face_network &init_class);
	~vgg_face_network();

	void process(float *input, int batch_size);
	TensorShape get_shape(int layer);
	TensorShape get_shape(std::string layername);
	float * get_result(int layer);
	float * get_result(std::string layername);

	void load_network();
	void unload_network();

	void print_meta();

	inline bool is_io_filled(){ return io_filled; }
private:
	std::string model_path;
	std::string meta_path;
	layer * layers;
	int layer_depth;
	int base_io_tensor_len;

	vl::DeviceType device_type;

	void build_model();
	int verify_model();

	bool is_loaded;
	std::vector<vgg_face_network*> * class_clones;
	bool io_filled;

	int io_batch_size;
	float * io_data;

	void fill_io_tensors(int batch_size);
	void empty_io_tensors();

	void * moveCPU(vl::Tensor tensor);
	void copyFromBinary(std::string filename, void *data, size_t size);
	void allocateMemoryFromBinary(std::string filename, void * &data, size_t size);
	
	vl::Context context;

	vl::Tensor vgg_conv_wrap(vl::Context &ctx, int layer_num);
	vl::Tensor vgg_fc_wrap(vl::Context &ctx, int layer_num, bool relu);
	vl::Tensor vgg_pool_wrap(vl::Context &ctx, int layer_num);
	vl::Tensor vgg_softmax_wrap(vl::Context &ctx, int layer_num);
	vl::Tensor embed_l2_norm_wrap(vl::Context &ctx, int layer_num);
};

#endif