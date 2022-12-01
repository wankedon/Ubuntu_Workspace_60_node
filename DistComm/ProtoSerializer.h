/**
 * @file ProtoSerializer.h
 * @brief protobuf message 序列化反序列化
 * @author 装备事业部软件组 杨青 
 * @version 0.1
 * @date 2020-09-08
 * 
 * @copyright Copyright (c) 2020  中国电子科技集团公司第四十一研究所
 * 
 */
#pragma once

/**
 * @brief protobuf message 串行化器
 * 
 * @tparam T 
 */
template <class T>
class ProtoSerializer
{
public:
/**
 * @brief 由proto message 构建串行化器
 * 
 * @param protoMessage 
 */
	ProtoSerializer(const T& protoMessage)
		:buffer(protoMessage.ByteSizeLong())
	{
		if (!buffer.empty())
		{
			protoMessage.SerializeToArray(buffer.data(), (int)buffer.size());
		}
		
	}

/**
 * @brief 串行化后的数据，一般用不到
 * 
 * @return std::pair<const byte*, size_t> 指向数据的指针和数据大小
 */
	std::pair<const byte*, size_t> data() const
	{
		return std::pair<const byte*, size_t>{buffer.data(), buffer.size()};
	}

/**
 * @brief 把串行化的数据添加到消息尾部
 * 
 * @param msg 待添加的消息目标
 * @return true 添加成功
 * @return false 添加失败
 */
	bool addToZMsg(zmsg_t* msg)
	{
		if (!buffer.empty())
		{
			zmsg_addmem(msg, buffer.data(), buffer.size());
			return true;
		}
		else
		{
			return false;
		}
		
	}

/**
 * @brief 把串行化的数据插入到消息头部
 * 
 * @param msg 待插入的消息目标
 * @return true 插入成功
 * @return false 插入失败
 */
	bool pushToZMsg(zmsg_t* msg)
	{
		if (!buffer.empty())
		{
			zmsg_pushmem(msg, buffer.data(), buffer.size());
			return true;
		}
		else
		{
			return false;
		}
	}
private:
	std::vector<byte> buffer;	///< 存放protobuf message 的缓冲区
};

/**
 * @brief protobuf message 的反串行化器
 * 
 * @tparam T 待复原的消息类型
 */
template <class T>
class ProtoDeserializer
{
public:
	/**
	 * @brief 根据数据指针和大小构建反串行化器，反串行化器只引用，不拷贝数据，注意数据的有效性
	 * 
	 * @param data 指向数据的指针
	 * @param size 数据大小
	 */
	ProtoDeserializer(const void* data, size_t size)
		:data(data),
		size(size)
	{

	}

	/**
	 * @brief 根据czmq的消息帧构建反串行化器
	 * 
	 * @param f 
	 */
	ProtoDeserializer(zframe_t* f)
		:data(nullptr),
		size(0)
	{
		if (f != nullptr)
		{
			data = zframe_data(f);
			size = zframe_size(f);
		}
	}

	/**
	 * @brief 反串行化
	 * 
	 * @param t 输出参数，如果解析成功，将字节流恢复为消息
	 * @return true 解析成功
	 * @return false 解析失败
	 */
	bool parse(T& t)
	{
		if (size == 0)
			return false;
		return t.ParseFromArray(data, (int)size);
	}
private:
	const void* data;	///< 指向数据的指针
	size_t size;	///< 数据大小
};