/**
 * @file ZMsgWrap.h
 * @brief zeromq��Ϣ���л�����
 * @author װ����ҵ�������� ���� 
 * @version 0.1
 * @date 2021-07-13
 * 
 * @copyright Copyright (c) 2021  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once

#include "ProtoSerializer.h"

std::string extractString(zmsg_t* msg);
template <class T>
bool extractFromMsgTop(zmsg_t* msg, T& item, bool erase)
{
	zframe_t* f = nullptr;
	if (erase)
	{
		f = zmsg_pop(msg);
	}
	else
	{
		f = zmsg_first(msg);
	}
	if (f == nullptr)
		return false;
	bool success = true;
	if (zframe_size(f) == sizeof(T))
	{
		memcpy(&item, zframe_data(f), zframe_size(f));
	}
	else
	{
		success = false;
	}
	if (erase)	//destroy frame by myself
	{
		zframe_destroy(&f);
	}
	return success;
}

template <class T>
bool extractFromMsgTop(zmsg_t* msg, std::vector<T>& items, bool erase)
{
	zframe_t* f = nullptr;
	if (erase)
	{
		f = zmsg_pop(msg);
	}
	else
	{
		f = zmsg_first(msg);
	}
	if (f == nullptr)
		return false;
	bool success = true;
	if (zframe_size(f) % sizeof(T) != 0)
	{
		success = false;
	}
	else
	{
		items = { (T*)zframe_data(f), (T*)zframe_data(f) + zframe_size(f) / sizeof(T) };
	}
	if (erase)	//destroy frame by myself
	{
		zframe_destroy(&f);
	}
	return success;
}

template <class T>
bool deserializeFromMsgTop(zmsg_t* msg, T& item, bool erase)
{
	zframe_t* f;
	if (erase)
	{
		f = zmsg_pop(msg);
	}
	else
	{
		f = zmsg_first(msg);
	}
	if (f == nullptr)
		return false;
	ProtoDeserializer<T> pd(f);
	auto rc = pd.parse(item);
	if (erase)
	{
		zframe_destroy(&f);
	}
	return rc;
}

class MessageBuilder
{
public:
	MessageBuilder(zmsg_t* msg) noexcept
		:msg(msg)
	{

	}
	MessageBuilder() noexcept
	{
		msg = zmsg_new();
	}

	MessageBuilder(const MessageBuilder& builder) = delete;
	MessageBuilder& operator=(const MessageBuilder& builder) = delete;

	MessageBuilder(MessageBuilder&& builder) noexcept
	{
		this->msg = builder.msg;
		builder.msg = nullptr;
	}

	~MessageBuilder()
	{
		if (msg != nullptr)
		{
			zmsg_destroy(&msg);
		}
	}
public:
	MessageBuilder& pushString(const std::string& str)
	{
		zmsg_pushstr(msg, str.c_str());
		return *this;
	}
	
	MessageBuilder& addString(const std::string& str)
	{
		zmsg_addstr(msg, str.c_str());
		return *this;
	}

	template <class T>
	MessageBuilder& add(const T& data)
	{
		zmsg_addmem(msg, &data, sizeof(T));
		return *this;
	}

	template <class T>
	MessageBuilder& push(const T& data)
	{
		zmsg_pushmem(msg, &data, sizeof(T));
		return *this;
	}

	template <class T>
	MessageBuilder& add(const std::vector<T>& data, size_t count = 0)
	{
		if (count == 0)
		{
			count = data.size();
		}
		zmsg_addmem(msg, data.data(), count * sizeof(T));
		return *this;
	}

	template<class T>
	MessageBuilder& serializeToTop(const T& data)
	{
		ProtoSerializer<T> ps(data);
		ps.pushToZMsg(msg);
		return *this;
	}

	template<class T>
	MessageBuilder& serializeToTail(const T& data)
	{
		ProtoSerializer<T> ps(data);
		ps.addToZMsg(msg);
		return *this;
	}

	/*zmsg_t* rawMsg()
	{
		assert(msg);
		auto result = msg;
		msg = nullptr;
		return result;
	}*/

	MessageBuilder clone()
	{
		return MessageBuilder(zmsg_dup(msg));
	}

	size_t size() const
	{
		return zmsg_size(msg);
	}

	template<class T>
	int send(T* sock) const
	{
		return zmsg_send(&msg, sock);
	}

	template<class T>
	int sendFrame(T* sock, uint32_t rid) const
	{
		auto* f = zmsg_encode(msg);	//������Ϣ��֡
		zframe_set_routing_id(f, rid);	//����֡·��
		return zframe_send(&f, sock, 0);
	}

	template<class T>
	int sendFrame(T* sock) const
	{
		auto* f = zmsg_encode(msg);	//������Ϣ��֡
		return zframe_send(&f, sock, 0);
	}
private:
	mutable zmsg_t* msg;
};

class MessageExtractor
{
public:
	MessageExtractor(zmsg_t* msg) noexcept
		:msg(msg)
	{
		assert(msg != nullptr);
	}

	MessageExtractor(MessageExtractor&& extractor) noexcept
	{
		this->msg = extractor.msg;
		extractor.msg = nullptr;
	}

	MessageExtractor(const MessageExtractor&) = delete;
	MessageExtractor& operator=(const MessageExtractor&) = delete;

	~MessageExtractor() noexcept
	{
		if (msg != nullptr)
		{
			zmsg_destroy(&msg);
		}
	}
public:
	static std::unique_ptr<MessageExtractor> createFromReceivedMsg(zsock_t* sock)
	{
		auto msg = zmsg_recv(sock);
		if (msg == nullptr)
		{
			return nullptr;
		}
		else
		{
			return std::make_unique<MessageExtractor>(msg);
		}
	}
public:
	size_t size() const
	{
		return zmsg_size(msg);
	}

	std::string popString()
	{
		return extractString(msg);
	}

	template <class T>
	bool extract(T& item, bool erase = true)
	{
		return extractFromMsgTop(msg, item, erase);
	}

	template <class T>
	bool extract(std::vector<T>& items, bool erase = true)
	{
		return extractFromMsgTop(msg, items, erase);
	}

	template <class T>
	bool deserialize(T& item, bool erase = true)
	{
		return deserializeFromMsgTop(msg, item, erase);
	}

	zmsg_t* dup()
	{
		return zmsg_dup(msg);
	}

	size_t firstFrameSize()
	{
		return zframe_size(zmsg_first(msg));
	}

	void* firstFrameRawData()	//���ã�ֻ�ڿ����ɱ��ߵ�ʱ����
	{
		return zframe_data(zmsg_first(msg));
	}
private:
	zmsg_t* msg;
};
