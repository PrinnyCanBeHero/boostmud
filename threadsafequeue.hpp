#ifndef __THREAD_SAFE_QUEUE_H_
#define __THREAD_SAFE_QUEUE_H_

#include <boost/thread.hpp>

namespace boostMUD {
namespace util {

/** A non-locking thread-safe Queue data structure.
 * If you wish to store pointer data types in this structure, I strongly suggest using std::unique_ptr objects to store them. */
template<class DataType>
class ThreadSafeQueue {
public:
	/** Sets up a queue for inter-thread communication. */
	ThreadSafeQueue()
			: producerRoot(new QueueNode()), consumerRoot(producerRoot) {
	}

	virtual ~ThreadSafeQueue() {
		while (consumerRoot != NULL) {
			QueueNode *temp = consumerRoot;
			consumerRoot = consumerRoot->next;
			delete temp;
		}
	}
	/* Consumer methods. */

	/** Returns if any data is available for the consumer. */
	bool dataAvailable() const {
		return (consumerRoot && consumerRoot->next);
	}
	/** Returns the next available piece of data from the queue. Note that calling this when !dataAvailable() is an error. */
	virtual DataType getData() throw (std::logic_error) {
		if (!dataAvailable()) {
			throw(std::logic_error("No data available in getData!"));
		}
		QueueNode *temp = consumerRoot;
		consumerRoot = consumerRoot->next;
		delete temp;

		return consumerRoot->data;
	}
	/** Allows the consumer to peek at the next data which is available */
	virtual const DataType& peek() const throw (std::logic_error) {
		if (!dataAvailable()) {
			throw(std::logic_error("No data available in getData!"));
		}
		return consumerRoot->next->data;
	}

	/*Producer methods. */

	/** Allows the producer to add data to the queue. */
	virtual void addData(DataType data) {

		QueueNode *temp = new QueueNode(data);

		producerRoot->next = temp;
		producerRoot = temp;

	}

	ThreadSafeQueue(const ThreadSafeQueue&) = delete; // we take it away so you dont hurt yourself
private:

	struct QueueNode {
		QueueNode(DataType d) : next(NULL), data(d) {}
		QueueNode() : next(NULL) {}
		~QueueNode() { }
		QueueNode* volatile next;
		DataType data;
	};

	QueueNode *producerRoot;
	QueueNode *consumerRoot;

};

/** Thread safe queue that adds a lock for multiple producers.
 * @see ThreadSafeQueue
 */
template<class DataType>
class MultiProducerThreadSafeQueue: public ThreadSafeQueue<DataType> {
public:
	MultiProducerThreadSafeQueue()
			: ThreadSafeQueue<DataType>() {
	}

	/** Locking function for producer to add data to the queue. */
	virtual void addData(DataType data) {
		boost::mutex::scoped_lock lock(m_mutex);
		ThreadSafeQueue<DataType>::addData(data);
	}
	virtual ~MultiProducerThreadSafeQueue() { } //To hide extraneous Eclipse warning

private:
	boost::mutex m_mutex;

};
/** Thread safe queue that adds a lock for multiple consumers
 * @see ThreadSafeQueue
 * @warning This class is probably not thread safe, as a thread could steal the data between calls to dataAvailable() and getData()
 */
template<class DataType>
class MultiConsumerThreadSafeQueue: public ThreadSafeQueue<DataType> {
public:
	MultiConsumerThreadSafeQueue()
			: ThreadSafeQueue<DataType>() {
	}
	virtual bool dataAvailable() {
		boost::mutex::scoped_lock lock(m_mutex);
		return ThreadSafeQueue<DataType>::dataAvailable();
	}

	virtual DataType getData() throw (std::logic_error) {
		boost::mutex::scoped_lock lock(m_mutex);
		return ThreadSafeQueue<DataType>::getData();
	}

	virtual const DataType& peek() const throw (std::logic_error) {
		boost::mutex::scoped_lock lock(m_mutex);
		return ThreadSafeQueue<DataType>::peek();
	}
	virtual ~MultiConsumerThreadSafeQueue() { } //To hide extraneous Eclipse warning

private:
	mutable boost::mutex m_mutex;
};

}

}
#endif
