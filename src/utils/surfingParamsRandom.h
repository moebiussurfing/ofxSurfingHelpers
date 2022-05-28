#pragma once
#include "ofMain.h"

#define DEFAULT_MIN_PCT 0.1f
#define DEFAULT_MAX_PCT 0.9f

//--

class SurfingParamsRandom
{
private:

	enum ResetPramsType
	{
		RESET_PARAM_MIN = 0,
		RESET_PARAM_FULL_CENTER,
		RESET_PARAM_RANGE_CENTER,
		RESET_PARAM_MAX,
		RESET_RANGE_MIN,
		RESET_RANGE_MAX,
	};

private:

	ofParameterGroup params{ "params" };//the external parameters or targets
	ofParameterGroup params_EditorGroups{ "editorGroups" };//the range limits to randomize each param
	ofParameterGroup params_EditorEnablers{ "editorEnablers" };//the enabled params to randomize
	vector<ofParameter<bool>> enablersForParams;

public:

	SurfingParamsRandom() {};
	~SurfingParamsRandom() {};
	
	//--------------------------------------------------------------
	void doRandom() 
	{
		doRandomize();
	};
	
	//--------------------------------------------------------------
	void doReset() {
		doResetParamsFull(RESET_PARAM_FULL_CENTER);
	};

	//--------------------------------------------------------------
	void setup(ofParameterGroup& group) {

		for (auto p : group) // ofAbstractParameter
		{
			//// exclude params not marked as serializable
			//if (!p->isSerializable()) continue;
			////if (!parameter->isSerializable()) return;

			//--

			// Filter different types

			auto parameterGroup = std::dynamic_pointer_cast<ofParameterGroup>(p);// group
			auto parameterFloat = std::dynamic_pointer_cast<ofParameter<float>>(p);
			auto parameterInt = std::dynamic_pointer_cast<ofParameter<int>>(p);
			auto parameterBool = std::dynamic_pointer_cast<ofParameter<bool>>(p);

			auto parameterVec2 = std::dynamic_pointer_cast<ofParameter<glm::vec2>>(p);
			auto parameterVec3 = std::dynamic_pointer_cast<ofParameter<glm::vec3>>(p);
			auto parameterVec4 = std::dynamic_pointer_cast<ofParameter<glm::vec4>>(p);

			//TODO:
			//auto parameteriVec2 = std::dynamic_pointer_cast<ofParameter<glm::ivec2>>(p);
			//auto parameteriVec3 = std::dynamic_pointer_cast<ofParameter<glm::ivec3>>(p);
			//auto parameteriVec4 = std::dynamic_pointer_cast<ofParameter<glm::ivec4>>(p);

			//---

			// Group Type

			if (parameterGroup)
			{
				// Use only the serializable params of the group
				//if (!parameterGroup->isSerializable()) continue;
				////if (!parameterGroup->isSerializable()) return;

				//TODO:
				//BUG: multiple nested..
				setup(*parameterGroup);

				//auto g0 = group.getGroup(parameterGroup->getName());
				//addGroup(g0);

				continue;
			}

			//-

			// Common Types

			//-

			// Bool
			else if (parameterBool && 1)
			{
				std::string _name = parameterBool->getName();

				ofParameterGroup _g{ _name };
				//ofParameter<bool> b{ "Enable " + _name, false };
				ofParameter<bool> p0 = group.getBool(_name);

				//_g.add(b);
				_g.add(p0);

				params_EditorGroups.add(_g);

				//-

				ofParameter<bool> b0{ _name, true };
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				continue;
			}

			//-

			// Int
			else if (parameterInt)
			{
				std::string _name = parameterInt->getName();

				int vtot = parameterInt->getMax() - parameterInt->getMin();
				int vmin = parameterInt->getMin() + DEFAULT_MIN_PCT * vtot;
				int vmax = parameterInt->getMin() + DEFAULT_MAX_PCT * vtot;

				ofParameterGroup _g{ _name };
				ofParameter<int> p0 = group.getInt(_name);
				ofParameter<int> pmin{ "Min", vmin, parameterInt->getMin() , parameterInt->getMax() };
				ofParameter<int> pmax{ "Max", vmax, parameterInt->getMin() , parameterInt->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				//-

				ofParameter<bool> b0{ _name, true };
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				continue;
			}

			//-

			// Float
			else if (parameterFloat)
			{
				std::string _name = parameterFloat->getName();

				float vtot = parameterFloat->getMax() - parameterFloat->getMin();
				float vmin = parameterFloat->getMin() + DEFAULT_MIN_PCT * vtot;
				float vmax = parameterFloat->getMin() + DEFAULT_MAX_PCT * vtot;

				ofParameterGroup _g{ _name };
				//ofParameter<bool> b{ "Enable " + _name, false };
				ofParameter<float> p0 = group.getFloat(_name);
				ofParameter<float> pmin{ "Min", vmin, parameterFloat->getMin() , parameterFloat->getMax() };
				ofParameter<float> pmax{ "Max", vmax, parameterFloat->getMin() , parameterFloat->getMax() };

				//_g.add(b);
				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				//-

				ofParameter<bool> b0{ _name, true };
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				continue;
			}

			//-

			// Vec2
			else if (parameterVec2)
			{
				auto vtot = parameterVec2->getMax() - parameterVec2->getMin();
				auto vmin = parameterVec2->getMin() + DEFAULT_MAX_PCT * vtot;
				auto vmax = parameterVec2->getMax() - (1 - DEFAULT_MAX_PCT) * vtot;

				std::string _name = parameterVec2->getName();
				ofParameterGroup _g{ _name };
				ofParameter<glm::vec2> p0 = group.getVec2f(_name);
				ofParameter<glm::vec2> pmin{ "Min", vmin, parameterVec2->getMin() , parameterVec2->getMax() };
				ofParameter<glm::vec2> pmax{ "Max", vmax, parameterVec2->getMin() , parameterVec2->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				ofParameter<bool> b0{ _name, true };
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				//ofParameter<bool> b0{ _name, true};//-> The global toggle for all the coords
				//ofParameter<bool> b0_x{ _name + " X", false };
				//ofParameter<bool> b0_y{ _name + " Y", false };
				//enablersForParams.push_back(b0);
				//params_EditorEnablers.add(b0);
				//enablersForParams.push_back(b0_x);
				//enablersForParams.push_back(b0_y);
				//params_EditorEnablers.add(b0_x);
				//params_EditorEnablers.add(b0_y);

				continue;
			}

			// Vec3
			else if (parameterVec3)
			{
				auto vtot = parameterVec3->getMax() - parameterVec3->getMin();
				auto vmin = parameterVec3->getMin() + DEFAULT_MAX_PCT * vtot;
				auto vmax = parameterVec3->getMax() - (1 - DEFAULT_MAX_PCT) * vtot;

				std::string _name = parameterVec3->getName();
				ofParameterGroup _g{ _name };
				ofParameter<glm::vec3> p0 = group.getVec3f(_name);
				ofParameter<glm::vec3> pmin{ "Min", vmin, parameterVec3->getMin() , parameterVec3->getMax() };
				ofParameter<glm::vec3> pmax{ "Max", vmax, parameterVec3->getMin() , parameterVec3->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				ofParameter<bool> b0{ _name, true };
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				//ofParameter<bool> b0{ _name, true};
				//ofParameter<bool> b0_x{ _name + " X", false };
				//ofParameter<bool> b0_y{ _name + " Y", false };
				//ofParameter<bool> b0_z{ _name + " Z", false };
				//enablersForParams.push_back(b0);
				//params_EditorEnablers.add(b0);
				//enablersForParams.push_back(b0_x);
				//enablersForParams.push_back(b0_y);
				//enablersForParams.push_back(b0_z);
				//params_EditorEnablers.add(b0_x);
				//params_EditorEnablers.add(b0_y);
				//params_EditorEnablers.add(b0_z);

				continue;
			}

			// Vec4
			else if (parameterVec4)
			{
				auto vtot = parameterVec4->getMax() - parameterVec4->getMin();
				auto vmin = parameterVec4->getMin() + DEFAULT_MAX_PCT * vtot;
				auto vmax = parameterVec4->getMax() - (1 - DEFAULT_MAX_PCT) * vtot;

				std::string _name = parameterVec4->getName();
				ofParameterGroup _g{ _name };
				ofParameter<glm::vec4> p0 = group.getVec4f(_name);
				ofParameter<glm::vec4> pmin{ "Min", vmin, parameterVec4->getMin() , parameterVec4->getMax() };
				ofParameter<glm::vec4> pmax{ "Max", vmax, parameterVec4->getMin() , parameterVec4->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				ofParameter<bool> b0{ _name, true };
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				//ofParameter<bool> b0{ _name, true};
				//ofParameter<bool> b0_x{ _name + " X", false };
				//ofParameter<bool> b0_y{ _name + " Y", false };
				//ofParameter<bool> b0_z{ _name + " Z", false };
				//ofParameter<bool> b0_w{ _name + " W", false };
				//enablersForParams.push_back(b0);
				//params_EditorEnablers.add(b0);
				//enablersForParams.push_back(b0_x);
				//enablersForParams.push_back(b0_y);
				//enablersForParams.push_back(b0_z);
				//enablersForParams.push_back(b0_w);
				//params_EditorEnablers.add(b0_x);
				//params_EditorEnablers.add(b0_y);
				//params_EditorEnablers.add(b0_z);
				//params_EditorEnablers.add(b0_w);

				continue;
			}

			//-

			//TODO:

			/*
			// iVec2
			else if (parameteriVec2)
			{
				auto vtot = parameteriVec2->getMax() - parameteriVec2->getMin();
				auto vmin = parameteriVec2->getMin();
				auto vmax = parameteriVec2->getMax();

				std::string _name = parameteriVec2->getName();
				ofParameterGroup _g{ _name };
				ofParameter<glm::vec2> p0 = group.getVec2f(_name);
				ofParameter<glm::vec2> pmin{ "Min", vmin, parameteriVec2->getMin() , parameteriVec2->getMax() };
				ofParameter<glm::vec2> pmax{ "Max", vmax, parameteriVec2->getMin() , parameteriVec2->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				ofParameter<bool> b0{ _name, true};
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				continue;
			}

			// iVec3
			else if (parameteriVec3)
			{
				auto vtot = parameteriVec3->getMax() - parameteriVec3->getMin();
				auto vmin = parameteriVec3->getMin();
				auto vmax = parameteriVec3->getMax();

				std::string _name = parameteriVec3->getName();
				ofParameterGroup _g{ _name };
				ofParameter<glm::vec3> p0 = group.getVec3f(_name);
				ofParameter<glm::vec3> pmin{ "Min", vmin, parameteriVec3->getMin() , parameteriVec3->getMax() };
				ofParameter<glm::vec3> pmax{ "Max", vmax, parameteriVec3->getMin() , parameteriVec3->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				ofParameter<bool> b0{ _name, true};
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				continue;
			}

			// iVec4
			else if (parameteriVec4)
			{
				auto vtot = parameteriVec4->getMax() - parameteriVec4->getMin();
				auto vmin = parameteriVec4->getMin();
				auto vmax = parameteriVec4->getMax();

				std::string _name = parameteriVec4->getName();
				ofParameterGroup _g{ _name };
				ofParameter<glm::vec4> p0 = group.getVec4f(_name);
				ofParameter<glm::vec4> pmin{ "Min", vmin, parameteriVec4->getMin() , parameteriVec4->getMax() };
				ofParameter<glm::vec4> pmax{ "Max", vmax, parameteriVec4->getMin() , parameteriVec4->getMax() };

				_g.add(p0);
				_g.add(pmin);
				_g.add(pmax);

				params_EditorGroups.add(_g);

				ofParameter<bool> b0{ _name, true};
				enablersForParams.push_back(b0);
				params_EditorEnablers.add(b0);

				continue;
			}
			*/

			//-

			ofLogWarning(__FUNCTION__) << "Could not create GUI element for parameter " << p->getName();
		}

		//----

		//TODO:
		//// create an enabler bool/toggle for each parameter
		//for (auto pb : enablersForParams)
		//{
		//	params_EditorEnablers.add(pb);
		//}
	}

	// Resets

private:

	//--------------------------------------------------------------
	void doResetParamsFull(ResetPramsType MS_type) {
		ofLogNotice(__FUNCTION__);


		for (int i = 0; i < enablersForParams.size(); i++)
			//for (auto p : enablersForParams)
		{
			auto p = enablersForParams[i];
			if (!p.get()) continue;//only reset this iterated param if it's enabled

			//-

			std::string name = p.getName();//name
			auto& g = params_EditorGroups.getGroup(name);//ofParameterGroup
			auto& e = g.get(name);//ofAbstractParameter

			auto type = e.type();
			bool isFloat = type == typeid(ofParameter<float>).name();
			bool isInt = type == typeid(ofParameter<int>).name();

			bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
			bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
			bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

			if (isFloat)
			{
				auto pmin = g.getFloat("Min").get();
				auto pmax = g.getFloat("Max").get();
				ofParameter<float> p0 = e.cast<float>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
			}

			else if (isInt)
			{
				auto pmin = g.getInt("Min").get();
				auto pmax = g.getInt("Max").get();
				ofParameter<int> p0 = e.cast<int>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
			}

			else if (isVec2)
			{
				auto pmin = g.getVec2f("Min").get();
				auto pmax = g.getVec2f("Max").get();
				ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

				//for (int dim = 0; dim < 2; dim++) {
				//	if (dim == 0) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.getMin().x, p0.get().y));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.getMax().x - p0.getMin().x / 2, p0.get().y));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(pmax.x - pmin.x / 2, p0.get().y));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(pmin.x, p0.get().y));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(pmax.x, p0.get().y));//reset to range max
				//	}
				//	else if (dim == 1) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.get().x, p0.getMin().y));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.get().x,p0.getMax().y - p0.getMin().y / 2));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(p0.get().x,pmax.y - pmin.y / 2));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(p0.get().x, pmin.y));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(p0.get().x, pmax.y));//reset to range max
				//	}
				//}
			}
			else if (isVec3)
			{
				auto pmin = g.getVec3f("Min").get();
				auto pmax = g.getVec3f("Max").get();
				ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

				//for (int dim = 0; dim < 3; dim++) {
				//	if (dim == 0) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.getMin().x, p0.get().y, p0.get().z));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.getMax().x - p0.getMin().x / 2, p0.get().y, p0.get().z));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(pmax.x - pmin.x / 2, p0.get().y, p0.get().z));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(pmin.x, p0.get().y, p0.get().z));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(pmax.x, p0.get().y, p0.get().z));//reset to range max
				//	}
				//	else if (dim == 1) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.getMin().y, p0.getMin().z));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.getMax().y - p0.getMin().y / 2, p0.getMin().z));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, pmax.y - pmin.y / 2, p0.getMin().z));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.getMin().z));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, pmin.y, p0.getMin().z));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, pmax.y, p0.getMin().z));//reset to range max
				//	}
				//	else if (dim == 2) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMin().z));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMax().z - p0.getMin().z / 2));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z - pmin.z / 2));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, pmin.z));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z));//reset to range max
				//	}
				//}
			}
			else if (isVec4)
			{
				auto pmin = g.getVec4f("Min").get();
				auto pmax = g.getVec4f("Max").get();
				ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

				//for (int dim = 0; dim < 4; dim++) {
				//	if (dim == 0) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.getMin().x, p0.get().y, p0.get().z, p0.get().w));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4((p0.getMax().x - p0.getMin().x) / 2, p0.get().y, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4((pmax.x - pmin.x) / 2, p0.get().y, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(pmin.x, p0.get().y, p0.get().z, p0.get().w));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(pmax.x, p0.get().y, p0.get().z, p0.get().w));//reset to range max
				//	}
				//	else if (dim == 1) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.getMin().y, p0.get().z, p0.get().w));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, (p0.getMax().y - p0.getMin().y) / 2, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, (pmax.y - pmin.y) / 2, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, pmin.y, p0.get().z, p0.get().w));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, pmax.y, p0.get().z, p0.get().w));//reset to range max
				//	}
				//	else if (dim == 2) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.getMin().z, p0.get().w));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (p0.getMax().z - p0.getMin().z) / 2, p0.get().w));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (pmax.z - pmin.z) / 2, p0.get().w));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, pmin.z, p0.get().w));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, pmax.z, p0.get().w));//reset to range max
				//	}
				//	else if (dim == 3) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMin()));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (p0.getMax().w - p0.getMin().w) / 2));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (pmax.w - pmin.w) / 2));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMax().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmin));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmax));//reset to range max
				//	}
				//}
			}
		}
	}

	//--------------------------------------------------------------
	void doResetRangesFull() {
		ofLogNotice(__FUNCTION__);

		for (auto p : enablersForParams)
		{
			if (!p.get()) continue;//only reset this param if it's enabled

			//-

			std::string name = p.getName();//name
			auto& g = params_EditorGroups.getGroup(name);//ofParameterGroup
			auto& e = g.get(name);//ofAbstractParameter

			auto type = e.type();
			bool isFloat = type == typeid(ofParameter<float>).name();
			bool isInt = type == typeid(ofParameter<int>).name();

			bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
			bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
			bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

			if (isFloat)
			{
				ofParameter<float> p0 = e.cast<float>();
				auto pmin = p0.getMin();
				auto pmax = p0.getMax();
				auto _pmin = g.get("Min").cast<float>();
				auto _pmax = g.get("Max").cast<float>();
				//reset to abs min
				_pmin.set(pmin);
				_pmax.set(pmax);
			}

			else if (isInt)
			{
				ofParameter<int> p0 = e.cast<int>();
				auto pmin = p0.getMin();
				auto pmax = p0.getMax();
				auto _pmin = g.get("Min").cast<int>();
				auto _pmax = g.get("Max").cast<int>();
				//reset to abs min
				_pmin.set(pmin);
				_pmax.set(pmax);
			}

			else if (isVec2)
			{
				ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();
				auto pmin = p0.getMin();
				auto pmax = p0.getMax();
				auto _pmin = g.get("Min").cast<glm::vec2>();
				auto _pmax = g.get("Max").cast<glm::vec2>();
				//reset to abs min
				_pmin.set(pmin);
				_pmax.set(pmax);
			}
			else if (isVec3)
			{
				ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();
				auto pmin = p0.getMin();
				auto pmax = p0.getMax();
				auto _pmin = g.get("Min").cast<glm::vec3>();
				auto _pmax = g.get("Max").cast<glm::vec3>();
				//reset to abs min
				_pmin.set(pmin);
				_pmax.set(pmax);
			}
			else if (isVec4)
			{
				ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();
				auto pmin = p0.getMin();
				auto pmax = p0.getMax();
				auto _pmin = g.get("Min").cast<glm::vec4>();
				auto _pmax = g.get("Max").cast<glm::vec4>();
				//reset to abs min
				_pmin.set(pmin);
				_pmax.set(pmax);
			}
		}
	}

	//--------------------------------------------------------------
	void doRandomize() {
		static int count = 0;
		ofLogNotice(__FUNCTION__) << count++;

		for (int i = 0; i < enablersForParams.size(); i++)
		{
			doRandomize(i, false);
		}
	}

	//--------------------------------------------------------------
	void doRandomize(int index, bool bForce) {
		ofLogVerbose(__FUNCTION__) << index;

		int i = index;

		//for (auto p : enablersForParams)
		//for (int i = 0; i<enablersForParams.size(); i++)
		{
			auto p = enablersForParams[i];

			if (!bForce)
				if (!p.get()) return;//only reset this param if it's enabled

			//-

			std::string name = p.getName();//name
			auto& g = params_EditorGroups.getGroup(name);//ofParameterGroup
			auto& e = g.get(name);//ofAbstractParameter

			auto type = e.type();
			bool isFloat = type == typeid(ofParameter<float>).name();
			bool isInt = type == typeid(ofParameter<int>).name();
			bool isBool = type == typeid(ofParameter<bool>).name();

			bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
			bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
			bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

			////TODO:
			//bool isiVec2 = type == typeid(ofParameter<glm::ivec2>).name();
			//bool isiVec3 = type == typeid(ofParameter<glm::ivec3>).name();
			//bool isiVec4 = type == typeid(ofParameter<glm::ivec4>).name();

			if (0) {}

			else if (isBool)
			{
				bool b = (ofRandom(0, 2) >= 1);
				ofParameter<bool> p0 = e.cast<bool>();
				p0.set(b);
			}

			else if (isInt)
			{
				auto pmin = g.getInt("Min").get();
				auto pmax = g.getInt("Max").get();
				ofParameter<int> p0 = e.cast<int>();
				p0.set((int)ofRandom(pmin, pmax + 1));//random
			}

			else if (isFloat)
			{
				auto pmin = g.getFloat("Min").get();
				auto pmax = g.getFloat("Max").get();
				ofParameter<float> p0 = e.cast<float>();
				p0.set((float)ofRandom(pmin, pmax));//random
			}

			else if (isVec2)
			{
				auto pvmin = g.getVec2f("Min").get();
				auto pvmax = g.getVec2f("Max").get();
				ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();

				float x = (float)ofRandom(pvmin.x, pvmax.x);
				float y = (float)ofRandom(pvmin.y, pvmax.y);
				//float x = p0.get().x;
				//float y = p0.get().y;
				//if (enablersForParams[i]) x = (float)ofRandom(pvmin.x, pvmax.x);
				//if (enablersForParams[i + 1]) y = (float)ofRandom(pvmin.y, pvmax.y);
				p0.set(glm::vec2(x, y));
			}
			else if (isVec3)
			{
				auto pvmin = g.getVec3f("Min").get();
				auto pvmax = g.getVec3f("Max").get();
				ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();

				float x = (float)ofRandom(pvmin.x, pvmax.x);
				float y = (float)ofRandom(pvmin.y, pvmax.y);
				float z = (float)ofRandom(pvmin.z, pvmax.z);
				//float x = p0.get().x;
				//float y = p0.get().y;
				//float z = p0.get().z;
				//if (enablersForParams[i]) x = (float)ofRandom(pvmin.x, pvmax.x);
				//if (enablersForParams[i + 1]) y = (float)ofRandom(pvmin.y, pvmax.y);
				//if (enablersForParams[i + 2]) z = (float)ofRandom(pvmin.z, pvmax.z);
				p0.set(glm::vec3(x, y, z));
			}
			else if (isVec4)
			{
				auto pvmin = g.getVec4f("Min").get();
				auto pvmax = g.getVec4f("Max").get();
				ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();

				float x = (float)ofRandom(pvmin.x, pvmax.x);
				float y = (float)ofRandom(pvmin.y, pvmax.y);
				float z = (float)ofRandom(pvmin.z, pvmax.z);
				float w = (float)ofRandom(pvmin.w, pvmax.w);

				//float x = p0.get().x;
				//float y = p0.get().y;
				//float z = p0.get().z;
				//float w = p0.get().w;
				//if (enablersForParams[i]) x = (float)ofRandom(pvmin.x, pvmax.x);
				//if (enablersForParams[i + 1]) y = (float)ofRandom(pvmin.y, pvmax.y);
				//if (enablersForParams[i + 2]) z = (float)ofRandom(pvmin.z, pvmax.z);
				//if (enablersForParams[i + 3]) w = (float)ofRandom(pvmin.w, pvmax.w);
				p0.set(glm::vec4(x, y, z, w));
			}

			//else if (isiVec2)
			//{
			//	auto pvmin = g.getVec2f("Min").get();
			//	auto pvmax = g.getVec2f("Max").get();
			//	ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();
			//	float x = (float)ofRandom(pvmin.x, pvmax.x);
			//	float y = (float)ofRandom(pvmin.y, pvmax.y);
			//	p0.set(glm::vec2(x, y));
			//}
			//else if (isVec3)
			//{
			//	auto pvmin = g.getVec3f("Min").get();
			//	auto pvmax = g.getVec3f("Max").get();
			//	ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();
			//	float x = (float)ofRandom(pvmin.x, pvmax.x);
			//	float y = (float)ofRandom(pvmin.y, pvmax.y);
			//	float z = (float)ofRandom(pvmin.z, pvmax.z);
			//	p0.set(glm::vec3(x, y, z));
			//}
			//else if (isVec4)
			//{
			//	auto pvmin = g.getVec4f("Min").get();
			//	auto pvmax = g.getVec4f("Max").get();
			//	ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();
			//	float x = (float)ofRandom(pvmin.x, pvmax.x);
			//	float y = (float)ofRandom(pvmin.y, pvmax.y);
			//	float z = (float)ofRandom(pvmin.z, pvmax.z);
			//	float w = (float)ofRandom(pvmin.w, pvmax.w);
			//	p0.set(glm::vec4(x, y, z, w));
			//}
		}
	}
};

