/*
 *******************************************************************************
 *
 * Purpose: Main loop implementation and Global objects container.
 *
 *******************************************************************************
 * Copyright Monstrenyatko 2014.
 *
 * Distributed under the MIT License.
 * (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *******************************************************************************
 */

/* Internal Includes */
#include "Application.h"
#include "CommandProcessor.h"
#include "SignalProcessor.h"
#include "SerialPort.h"
#include "XBeeNet.h"
#include "TcpNet.h"
#include "Router.h"
/* External Includes */
/* System Includes */
#include <iostream>
#include <assert.h>


Application* Application::mInstance = NULL;

void Application::initialize()
throw (Utils::Error)
{
	assert(!mInstance);
	mInstance = new Application();
}

void Application::destroy()
throw ()
{
	delete mInstance;
	mInstance = NULL;
}

Application::Application()
throw (Utils::Error)
:
	mSem(0, 1),
	mProcessor(NULL),
	mSignalProcessor(NULL),
	mSerial(NULL),
	mXBeeNet(NULL),
	mTcpNet(NULL),
	mRouter(NULL)
{
	std::unique_ptr<Utils::CommandProcessor> ptrProcessor;
	std::unique_ptr<SignalProcessor> ptrSignalProcessor;
	std::unique_ptr<SerialPort> ptrSerial;
	std::unique_ptr<XBeeNet> ptrXBeeNet;
	std::unique_ptr<TcpNet> ptrTcpNet;
	std::unique_ptr<Router> ptrRouter;

	// initialize objects in exception-save mode
	try {
		std::cout<<"INITIALIZATION"<<std::endl;
		ptrProcessor.reset(new Utils::CommandProcessor);
		ptrSignalProcessor.reset(new SignalProcessor);
		ptrSerial.reset(new SerialPort("/dev/tty.usbserial-AH031GNH", 57600));
		ptrXBeeNet.reset(new XBeeNet());
		ptrTcpNet.reset(new TcpNet());
		ptrRouter.reset(new Router);
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}

	// start all services
	std::cout<<"START"<<std::endl;
	try {
		ptrProcessor->start();
		ptrSignalProcessor->start();
		ptrSerial->start();
		ptrXBeeNet->start();
		ptrTcpNet->start();
		ptrRouter->start();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}

	// no exceptions from this point
	mProcessor = ptrProcessor.release();
	mSignalProcessor = ptrSignalProcessor.release();
	mSerial = ptrSerial.release();
	mXBeeNet = ptrXBeeNet.release();
	mTcpNet = ptrTcpNet.release();
	mRouter = ptrRouter.release();
}

Application::~Application()
throw ()
{
	// stop all services
	std::cout<<"STOP"<<std::endl;
	try {
		mRouter->stop();
		mTcpNet->stop();
		mXBeeNet->stop();
		mSerial->stop();
		mSignalProcessor->stop();
		mProcessor->stop();
	} catch (std::exception& e) {
		throw Utils::Error(e, UTILS_STR_CLASS_FUNCTION(Application));
	}

	// clean objects
	std::cout<<"DESTROY"<<std::endl;
	try {
		delete mRouter;
		delete mTcpNet;
		delete mXBeeNet;
		delete mSerial;
		delete mSignalProcessor;
		delete mProcessor;
	} catch (std::exception& e) {
		std::cerr<<UTILS_STR_CLASS_FUNCTION(Application)<<std::endl;
	}
}

void Application::run() throw () {
	std::cout<<"PROCESSING"<<std::endl;
	// wait request to finish
	mSem.wait();
	std::cout<<"FINISHING"<<std::endl;
}

void Application::stop() throw () {
	mSem.post();
}
