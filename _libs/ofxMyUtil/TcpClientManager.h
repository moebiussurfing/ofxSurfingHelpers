#pragma once
#include "AbsCommunicationManager.h"
#include "ofxNetwork.h"
#include "ofUtils.h"

namespace ofxMyUtil 
{
	class TcpClientManager : public AbsCommunicationManager
	{
	public:

		TcpClientManager(std::shared_ptr<ofxTCPClient> tcp) { this->mTcp = tcp; }
		~TcpClientManager() { Stop(); }

		//--------------------------------------------------------------
		void Start(std::string ip, int port, std::string delimiter = "\n", bool autoRedirect = false) 
		{	
			if (isThreadRunning()) 
			{
				mLogGui.AddText("[Warning] : You tried start thread but thread has already running.");
				return;
			}

			mIpAddress = ip;
			mPort = port;
			mAutoRedirect = autoRedirect;
			mDelimiter = delimiter;

			startThread();
		}

		//--------------------------------------------------------------
		void Stop() 
		{
			if (isThreadRunning()) 
			{
				stopThread();
				waitForThread();
			}
			if (mTcp->isConnected()) mTcp->close();
		}

		//--------------------------------------------------------------
		void sendMsg(std::string msg) {
			
			if (mTcp->isConnected()) 
			{
				mLogGui.AddText("[Send] : " + msg);
				mTcp->send(msg);
			}
			else 
			{
				mLogGui.AddText("[Warning] : You tryed to send messages but now is not connecting to server.");
			}
		}

		//--------------------------------------------------------------
		void addProcessInThreadedFunction() override 
		{
			try
			{
				if (mTcp->isConnected()) 
				{
					std::string str = mTcp->receive();
					if (0 < str.length()) 
					{
						mLogGui.AddText("[Recieve] " + str);
						ofNotifyEvent(onReceiveNewMsg, str, this);
					}
				}
				else 
				{
					mDeltaTime = ofGetElapsedTimeMillis() - mConnectTime;

					// Redirect
					if (mDeltaTime > mRetryTime && mAutoRedirect) 
					{
						mLogGui.AddText("[Notice] : try connect to server...");

						ofxTCPSettings settings(mIpAddress, mPort);
						settings.messageDelimiter = mDelimiter;
						mTcp->setup(settings);

						if (mTcp->isConnected()) 
						{
							mLogGui.AddText("[Notice] : Success connect to server.");
							ofNotifyEvent(onConnectResult, true, this);
						}
						else 
						{
							mLogGui.AddText("[ERROR] : Failed connect to server.");
							ofNotifyEvent(onConnectResult, false, this);
						}
						mConnectTime = ofGetElapsedTimeMillis();
					}
				}
			}
			catch (const std::exception&)
			{
				mLogGui.AddText("[ERROR] : Unknown error");
			}
		}

		//--------------------------------------------------------------
		ofEvent<const std::string> onReceiveNewMsg;
		ofEvent<const bool> onConnectResult;

	private:
		void addPropertyImGui() override 
		{
			if (ImGui::CollapsingHeader("Settings Data")) 
			{
				ImGui::Text("Tcp server ip : %s, port : %i", mIpAddress.c_str(), mPort);
				ImGui::Text("Tcp server isConnect : %s", std::string(mTcp->isConnected() ? "connected" : "not connected").c_str());
				ImGui::Checkbox("Tcp connect auto redirect", &mAutoRedirect);
				ImGui::InputInt("Retry time", &mRetryTime, 100, 100000);
			}
		}

		std::shared_ptr<ofxTCPClient> mTcp;
		std::string mIpAddress = "127.0.0.1";
		int mPort = 8080;
		std::string mDelimiter = "\n";
		bool mAutoRedirect = false;
		uint64_t mDeltaTime = 0, mConnectTime = 0;
		int mRetryTime = 5000;

	};
}