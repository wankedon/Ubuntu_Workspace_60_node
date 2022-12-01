#pragma once

#include <vector>
template<typename T>
class StreamIn
{
public:
	virtual void receive(const T& data)
	{
		std::cout << "unimplement stream in" << std::endl;
	}
	virtual void receive(T& data)
	{
		receive((const T&)data);
	}
};

template <typename S>
class StreamOut
{
	std::vector<StreamIn<S>*> downstreams;

public:

	void send(const S& data)
	{
		for (auto ds : downstreams) ds->receive(data);
	}

	void send(S& data)
	{
		if (downstreams.size() == 0) return;

		int sz1 = downstreams.size() - 1;

		for (auto& ds : downstreams)
			ds->receive(data);
	}

	void connect(StreamIn<S>* s)
	{
		downstreams.push_back(s);
	}

	void swap(StreamOut<S>& s)
	{
		downstreams.swap(s.downstreams);
	}
};

template <typename inType, typename outType>
class StreamInOut : public StreamOut<outType>, public StreamIn<inType>
{
};

template <typename S>
inline StreamIn<S>& operator>>(StreamOut<S>& a, StreamIn<S>& b) { a.connect(&b); return b; }
template <typename T, typename S>
inline StreamInOut<S, T>& operator>>(StreamOut<S>& a, StreamInOut<S, T>& b) { a.connect(&b); return b; }

template <typename T, typename S>
inline StreamIn<S>& operator>>(StreamInOut<T, S>& a, StreamIn<S>& b) { a.connect(&b); return b; }
template <typename T, typename S, typename U>
inline StreamInOut<S, U>& operator>>(StreamInOut<T, S>& a, StreamInOut<S, U>& b) { a.connect(&b); return b; }


