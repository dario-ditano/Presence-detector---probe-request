/**
 * Copyright (c) 2019, HypnoProject
 * BLOCKING QUEUE
 *
 * Bonafede Giacomo
 * Ditano Dario
 * Poppa Emanuel
 *
 */

 /****************************
 * INCLUDE, DEFINE, MACRO   *
 ****************************/
 /*************************************************  INCLUDE  **********************************************************/

#include "stdafx.h"
#include "BlockingQueue.h"

/*****************************************************
* CONSTANTS, STRUCTS, GLOBAL VARIABLES AND FUNCTIONS *
******************************************************/
/************************************************  FUNCTIONS  *********************************************************/
/* Destructor */
template <typename T>
BlockingQueue<T>::~BlockingQueue() {
	invalidate();
}

/* Pop from the queue in a thread-safe way. Blocks execution until a value is available. Return true on success */
template <typename T>
bool BlockingQueue<T>::waitPop(T& out) {
	std::unique_lock<std::mutex> lock(mutex);
	// block if data is not available
	condition.wait(lock, [this]() {
		return !queue.empty() || !is_valid;
		});
	// exit if queue is not valid
	if (!is_valid) {
		return false;
	}
	out = std::move(queue.front());
	queue.pop();
	return true;
}

/* Push a new value into the queue. */
template <typename T>
void BlockingQueue<T>::push(T value) {
	std::lock_guard<std::mutex> lock(mutex);
	queue.push(std::move(value));
	condition.notify_one();
}

/* Clear all items from the queue. */
template <typename T>
void BlockingQueue<T>::clear() {
	std::lock_guard<std::mutex> lock(mutex);
	while (!queue.empty()) {
		queue.pop();
	}
	condition.notify_all();
}

/* Returns whether or not this queue is valid. */
template <typename T>
bool BlockingQueue<T>::isValid() {
	std::lock_guard<std::mutex> lock(mutex);
	return is_valid;
}

/* Check whether or not the queue is empty. */
template <typename T>
bool BlockingQueue<T>::empty() {
	std::lock_guard<std::mutex> lock(mutex);
	return queue.empty();
}

/* Invalidate the queue. Used to ensure no conditions are being waited on in waitPop when a thread or the application is trying to exit. */
template <typename T>
void BlockingQueue<T>::invalidate() {
	std::lock_guard<std::mutex> lock(mutex);
	is_valid = false;
	condition.notify_all();
}