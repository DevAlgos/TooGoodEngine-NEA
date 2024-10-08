#include "ScriptingEngine.h"

#include <emmintrin.h>

namespace TooGoodEngine {

	static ScriptingEngineData* s_Data;


	void ScriptingEngine::Init(const std::filesystem::path& path)
	{
		s_Data = new ScriptingEngineData();

		//
		// ---- Python initalization ----
		//

		Py_Initialize();

		std::filesystem::path currentDirectory = path;

		s_Data->ScriptDirectory = path;

		PyObject* sysModule = PyImport_ImportModule("sys");
		PyObject* sysPath   = PyObject_GetAttrString(sysModule, "path");

		std::filesystem::path pathToModule = __FILE__;
		pathToModule = pathToModule.parent_path();
		pathToModule = pathToModule / "Module";
		
		std::string strPathToModule = pathToModule.string();
		PyList_Append(sysPath, PyUnicode_FromString(strPathToModule.c_str()));

		std::string stringCurrentDirectory = currentDirectory.string();
		PyList_Append(sysPath, PyUnicode_FromString(stringCurrentDirectory.c_str()));

		Py_XDECREF(sysPath);
		Py_XDECREF(sysModule);

		//NOTE: remember to update MethodCount when adding new methods

		s_Data->MethodDefinitions[0] = { "log",         PythonBindings::LogMsg,   METH_VARARGS, "prints to console"};
		s_Data->MethodDefinitions[1] = { "log_error",   PythonBindings::LogError, METH_VARARGS, "prints error to console" };
		s_Data->MethodDefinitions[2] = { "log_warning", PythonBindings::LogWarn,  METH_VARARGS, "prints warning to console" };


		s_Data->MethodDefinitions[3] = { "is_key_pressed",  PythonBindings::IsKeyPressed,  METH_VARARGS, "returns true if a key is pressed" };
		s_Data->MethodDefinitions[4] = { "is_key_released", PythonBindings::IsKeyReleased, METH_VARARGS, "returns true if a key is released" };
		s_Data->MethodDefinitions[5] = { "is_key_down",     PythonBindings::IsKeyDown,	   METH_VARARGS, "returns true if a key is down" };
		
		s_Data->MethodDefinitions[6] = { "is_mouse_button_pressed",  PythonBindings::IsMouseButtonPressed,  METH_VARARGS, "returns true if a mouse button is pressed" };
		s_Data->MethodDefinitions[7] = { "is_mouse_button_released", PythonBindings::IsMouseButtonReleased, METH_VARARGS, "returns true if a mouse button is released" };
		s_Data->MethodDefinitions[8] = { "is_mouse_button_down",	 PythonBindings::IsMouseButtonDown,	    METH_VARARGS, "returns true if a mouse button is down" };

		s_Data->MethodDefinitions[9]  = { "get_mouse_coordinates", PythonBindings::GetMouseCoordinates, METH_NOARGS, "returns a tuple containing the mouse coordinates" };
		s_Data->MethodDefinitions[10] = { "get_scroll_wheel",      PythonBindings::GetScrollWheel,	    METH_NOARGS, "returns a tuple of scroll wheel x,y" };

		s_Data->MethodDefinitions[11] = { "disable_cursor", PythonBindings::DisableCursor, METH_NOARGS,  "hides and locks the cursor" };
		s_Data->MethodDefinitions[12] = { "enable_cursor",  PythonBindings::EnableCursor,  METH_NOARGS,  "returns the cursor to normal" };


		s_Data->MethodDefinitions[13] = { "internal_create_entity",				   PythonBindings::InternalCreateEntity,			  METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[14] = { "internal_get_entity_name",              PythonBindings::InternalGetEntityName,		      METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[15] = { "internal_get_entity_by_name",           PythonBindings::InternalGetEntityByName,		      METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[16] = { "internal_get_entity_id",			       PythonBindings::InternalGetEntityID,		          METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[17] = { "internal_add_component_to_entity",      PythonBindings::InternalAddComponentToEntity,      METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[18] = { "internal_remove_component_from_entity", PythonBindings::InternalRemoveComponentFromEntity, METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[19] = { "internal_remove_entity_from_tree",      PythonBindings::InternalRemoveEntityFromTree,      METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[20] = { "internal_get_component_from_entity",    PythonBindings::InternalGetComponentFromEntity,    METH_VARARGS, "internal" };
		
		s_Data->MethodDefinitions[21] = { "internal_translate_transform",			 PythonBindings::InternalTranslateTransform,		  METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[22] = { "internal_rotate_transform",				 PythonBindings::InternalRotateTransform,			  METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[23] = { "internal_scale_transform",				 PythonBindings::InternalScaleTransform,		      METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[24] = { "internal_get_translation_from_transform", PythonBindings::InternalGetTranslationFromTransform, METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[25] = { "internal_get_rotation_from_transform",    PythonBindings::InternalGetRotationFromTransform,    METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[26] = { "internal_get_scale_from_transform",       PythonBindings::InternalGetScaleFromTransform,       METH_VARARGS, "internal" };

		s_Data->MethodDefinitions[27] = { "internal_update_material_attribute",   PythonBindings::InternalUpdateMaterialAttribute,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[28] = { "internal_get_material_attribute",      PythonBindings::InternalGetMaterialAttribute,      METH_VARARGS, "internal" };
		
		s_Data->MethodDefinitions[29] = { "internal_translate_point_light",		  PythonBindings::InternalTranslatePointLight,       METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[30] = { "internal_update_point_light_color",	  PythonBindings::InternalUpdatePointLightColor,     METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[31] = { "internal_update_point_light_intensity",PythonBindings::InternalUpdatePointLightIntensity, METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[32] = { "internal_update_point_light_radius",   PythonBindings::InternalUpdatePointLightRadius,    METH_VARARGS, "internal" };

		s_Data->MethodDefinitions[33] = { "internal_update_directional_light_direction", PythonBindings::InternalUpdateDirectionalLightDirection, METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[34] = { "internal_update_directional_light_color",     PythonBindings::InternalUpdateDirectionalLightColor,     METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[35] = { "internal_update_directional_light_intensity", PythonBindings::InternalUpdateDirectionalLightIntensity, METH_VARARGS, "internal" };

		s_Data->MethodDefinitions[36] = { "internal_create_orthographic_camera_controller",			 PythonBindings::InternalCreateOrthographicCameraController,		METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[37] = { "internal_set_bottom_orthographic_camera_controller",		 PythonBindings::InternalSetBottomOrthographicCameraController,		METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[38] = { "internal_set_top_orthographic_camera_controller",		 PythonBindings::InternalSetTopOrthographicCameraController,		METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[39] = { "internal_set_left_orthographic_camera_controller",		 PythonBindings::InternalSetLeftOrthographicCameraController,		METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[40] = { "internal_set_right_orthographic_camera_controller",       PythonBindings::InternalSetRightOrthographicCameraController,		METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[41] = { "internal_update_position_orthographic_camera_controller", PythonBindings::InternalUpdatePositionOrthographicCameraController,METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[42] = { "internal_update_up_orthographic_camera_controller",		 PythonBindings::InternalUpdateUpOrthographicCameraController,	    METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[43] = { "internal_update_front_orthographic_camera_controller",	 PythonBindings::InternalUpdateFrontOrthographicCameraController,   METH_VARARGS, "internal" };

		s_Data->MethodDefinitions[44] = { "internal_create_perspective_camera_controller",   PythonBindings::InternalCreatePerpsectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[45] = { "internal_set_fov_perspective_camera_controller",	 PythonBindings::InternalSetFovPerspectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[46] = { "internal_set_aspect_ratio_perspective_camera_controller",	 PythonBindings::InternalSetAspectRatioPerpspectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[47] = { "internal_set_near_perspective_camera_controller",	 PythonBindings::InternalSetNearPerspectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[48] = { "internal_set_far_perspective_camera_controller",	 PythonBindings::InternalSetFarPerspectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[49] = { "internal_update_position_perspective_camera_controller",	 PythonBindings::InternalUpdatePositionPerspectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[50] = { "internal_update_up_perspective_camera_controller",	 PythonBindings::InternalUpdateUpPerspectiveCameraController,   METH_VARARGS, "internal" };
		s_Data->MethodDefinitions[51] = { "internal_update_front_perspective_camera_controller",	 PythonBindings::InternalUpdateFrontPerspectiveCameraController,   METH_VARARGS, "internal" };


		// ---- END ----
		s_Data->MethodDefinitions[52] = { nullptr, nullptr, 0, nullptr };
		
		s_Data->EngineModuleDefinition.m_base    = PyModuleDef_HEAD_INIT;
		s_Data->EngineModuleDefinition.m_name    = "TooGoodEngine";
		s_Data->EngineModuleDefinition.m_doc     = nullptr;
		s_Data->EngineModuleDefinition.m_size    =  -1;
		s_Data->EngineModuleDefinition.m_methods = s_Data->MethodDefinitions.data();


		s_Data->EngineModule = PyModule_Create(&s_Data->EngineModuleDefinition);
		
		if(!s_Data->EngineModule)
			PyErr_Print();


		PyObject* moduleDict = PyImport_GetModuleDict();
		PyDict_SetItemString(moduleDict, "TooGoodEngine", s_Data->EngineModule);
		Py_XDECREF(moduleDict);

		s_Data->Initialized = true;

	}
	void ScriptingEngine::Shutdown()
	{
		Py_XDECREF(s_Data->EngineModule);
		Py_Finalize();
		delete s_Data;
	}
	ScriptData ScriptingEngine::ExtractScript(const std::filesystem::path& path)
	{
		ScriptData data{};

		//converts from native system file path e.x C:\\...\\...\\Assets\\hello\\hello.py
		//to python format hello.hello

		std::filesystem::path relative = std::filesystem::relative(path, s_Data->ScriptDirectory);
		std::string stringPath = relative.string();

		size_t last = stringPath.find_last_of(".");
		
		//invalid file
		if (last == std::string::npos)
			return data;

		//clip the extension
		stringPath = stringPath.substr(0, last);

		//replaces seperator with .
		size_t i = 0;
		while (i < stringPath.length())
		{
			i = stringPath.find(path.preferred_separator, i);

			if (i != std::string::npos)
			{
				stringPath.replace(i, ScriptingEngineData::DelimiterSize, ScriptingEngineData::Delimiter);
				i += ScriptingEngineData::DelimiterSize;
			}
		}

		const char* p = stringPath.c_str();
		PyObject* module = PyImport_ImportModule(p);

		if (!module)
		{
			TGE_LOG_ERROR("failed to load module ", path);
			PyErr_Print();
			return data;
		}

		PyObject* pyOnCreate = PyObject_GetAttrString(module, "on_create");
		PyObject* pyOnUpdate = PyObject_GetAttrString(module, "on_update");

		//ownership is passed onto the user (Script Component)
		if (pyOnCreate && PyCallable_Check(pyOnCreate))
			data.PyOnCreate = pyOnCreate;
		else
			PyErr_Print();

		if (pyOnUpdate && PyCallable_Check(pyOnUpdate))
			data.PyOnUpdate = pyOnUpdate;
		else
			PyErr_Print();

		Py_XDECREF(module);
		return data;
	}
	ScriptData ScriptingEngine::CreateModuleFromData(const char* name, const char* data)
	{
		ScriptData scriptData{};

		//TODO:

		return scriptData;
	}
}