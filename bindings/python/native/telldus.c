#include "Python.h"
#include "datetime.h"
#include <telldus-core.h>
#include <stdio.h>

/* estrdup.c -- duplicate a string, die if error
 * 
 * char  *string;
 * char  *newstring;
 * newstring = estrdup(string);
 * 
 * estrdup returns a copy of its argument, located in memory
 * allocated from the heap.  If it is unable to allocate the
 * necessary memory, estrdup executes PyErr_NoMemory(); 
 * (Generally, the routine error is not expected to return,
 * but if it does, estrdup will return NULL.)
 */

char *
estrdup(char *s)
{
	register char  *t;

	if (NULL == (t = malloc(strlen(s)+1))) {
		return PyErr_NoMemory();
	}
	strcpy(t, s);
	return(t);
}

static PyObject *
telldus_tdInit(PyObject *self)
{
	tdInit();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
telldus_tdClose(PyObject *self)
{
	tdClose();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
telldus_tdTurnOn(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	return PyBool_FromLong((long) tdTurnOn(id));
}

static PyObject *
telldus_tdTurnOff(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	return PyBool_FromLong((long) tdTurnOff(id));
}

static PyObject *
telldus_tdBell(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	return PyBool_FromLong((long) tdBell(id));
}

static PyObject *
telldus_tdDim(PyObject *self, PyObject *args)
{
	long id;
	unsigned char level;

	if (!PyArg_ParseTuple(args, "lb", &id, &level))
		return NULL;
	
	if (level < 0 || level > 255)
		return NULL;

	return PyBool_FromLong((long) tdDim(id, level));
}

static PyObject *
telldus_tdLearn(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	return PyBool_FromLong((long) tdLearn(id));
}

static PyObject *
telldus_tdMethods(PyObject *self, PyObject *args)
{
	long id;
	long supportedmethods;

	if (!PyArg_ParseTuple(args, "ll", &id, &supportedmethods))
		return NULL;

	return PyLong_FromLong((long) tdMethods(id, supportedmethods));
}

	
static PyObject *
telldus_tdLastSentCommand(PyObject *self, PyObject *args)
{
	long id;
	long supportedmethods;

	if (!PyArg_ParseTuple(args, "ll", &id, &supportedmethods))
		return NULL;

	return PyLong_FromLong((long) tdLastSentCommand(id, supportedmethods));
}

static PyObject *
telldus_tdLastSentValue(PyObject *self, PyObject *args)
{
	long id;
	char* value;
	char* retval;
	
	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	value = tdLastSentValue(id);
	retval = estrdup(value);
	tdReleaseString(value);
	return PyString_FromString(retval);
}

static PyObject *
telldus_tdGetNumberOfDevices(PyObject *self)
{
	return PyLong_FromLong((long) tdGetNumberOfDevices());
}

static PyObject *
telldus_tdGetDeviceId(PyObject *self, PyObject *args)
{
	long index;

	if (!PyArg_ParseTuple(args, "l", &index))
		return NULL;
	
	return PyLong_FromLong((long) tdGetDeviceId(index));
}

static PyObject *
telldus_tdGetDeviceType(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	return PyLong_FromLong((long) tdGetDeviceType(id));
}

static PyObject *
telldus_tdGetErrorString(PyObject *self, PyObject *args)
{
	long errorno;
	char* errorString;
	char* retval;
	
	if (!PyArg_ParseTuple(args, "l", &errorno))
		return NULL;
	
	errorString = tdGetErrorString(errorno);
	retval = estrdup(errorString);
	tdReleaseString(errorString);
	return PyString_FromString(retval);
}

static PyObject *
telldus_tdGetName(PyObject *self, PyObject *args)
{
	long id;
	char* name;
	char* retval;
	
	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	name = tdGetName(id);
	retval = estrdup(name);
	tdReleaseString(name);
	return PyString_FromString(retval);
}

static PyObject *
telldus_tdSetName(PyObject *self, PyObject *args)
{
	long id;
	char* name;
	
	if (!PyArg_ParseTuple(args, "ls", &id, &name))
		return NULL;
	
	return PyBool_FromLong((long) tdSetName(id, name));
}

static PyObject *
telldus_tdGetProtocol(PyObject *self, PyObject *args)
{
	long id;
	char* protocol;
	char* retval;
	
	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	protocol = tdGetProtocol(id);
	retval = estrdup(protocol);
	tdReleaseString(protocol);
	return PyString_FromString(retval);
}

static PyObject *
telldus_tdSetProtocol(PyObject *self, PyObject *args)
{
	long id;
	char* protocol;
	
	if (!PyArg_ParseTuple(args, "ls", &id, &protocol))
		return NULL;
	
	return PyBool_FromLong((long) tdSetProtocol(id, protocol));

}

static PyObject *
telldus_tdGetModel(PyObject *self, PyObject *args)
{
	long id;
	char* model;
	char* retval;
	
	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	model = tdGetModel(id);
	retval = estrdup(model);
	tdReleaseString(model);
	return PyString_FromString(retval);
}

static PyObject *
telldus_tdSetModel(PyObject *self, PyObject *args)
{
	long id;
	char* model;
	
	if (!PyArg_ParseTuple(args, "ls", &id, &model))
		return NULL;
	
	return PyBool_FromLong((long) tdSetProtocol(id, model));

}

static PyObject *
telldus_tdGetDeviceParameter(PyObject *self, PyObject *args)
{
	long id;
	char* name;
	char* defaultValue;
	char* param;
	char* retval;
	
	if (!PyArg_ParseTuple(args, "lss", &id, &name, &defaultValue))
		return NULL;
	
	param = tdGetDeviceParameter(id, name, defaultValue);
	retval = estrdup(param);
	tdReleaseString(param);
	return PyString_FromString(retval);
}

static PyObject *
telldus_tdSetDeviceParameter(PyObject *self, PyObject *args)
{
	long id;
	char* name;
	char* value;
	
	if (!PyArg_ParseTuple(args, "lss", &id, &name, &value))
		return NULL;
	
	return PyBool_FromLong((long) tdSetDeviceParameter(id, name, value));
}

static PyObject *
telldus_tdAddDevice(PyObject *self)
{
	return PyLong_FromLong((long) tdAddDevice());
}

static PyObject *
telldus_tdRemoveDevice(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id))
		return NULL;
	
	return PyBool_FromLong((long) tdRemoveDevice(id));
}

static PyObject *
telldus_tdSendRawCommand(PyObject *self, PyObject *args)
{
	char *command;
	long reserved = 0;

	if (!PyArg_ParseTuple(args, "s|l", &command, &reserved));
		return NULL;

	return PyLong_FromLong((long) tdSendRawCommand(command, reserved));
}


void
telldus_deviceEventCallback(int deviceId, int method, const char *data, int callbackId, void *pyfunc)
{

	PyObject * result; 
	PyGILState_STATE gstate;
	
	gstate = PyGILState_Ensure();

	// now call the Python callback function
	result = PyObject_CallFunction(pyfunc, "llsl", deviceId, method, data, callbackId);

	if (result == NULL) {
		// something went wrong so print to stderr
		PyErr_Print();
	}

	// take care of reference handling
	Py_XDECREF(result);
	
	PyGILState_Release(gstate);
	
	return;
}
	
void
telldus_deviceChangeEventCallback(int deviceId, int changeEvent, int changeType, int callbackId, void *pyfunc)
{
	PyObject * result; 
	PyGILState_STATE gstate;
	
	gstate = PyGILState_Ensure();

	// now call the Python callback function
	result = PyObject_CallFunction(pyfunc, "llll", deviceId, changeEvent, changeType, callbackId);

	if (result == NULL) {
		// something went wrong so print to stderr
		PyErr_Print();
	}

	// take care of reference handling
	Py_XDECREF(result);
	
	PyGILState_Release(gstate);
	
	return;   
}
	
void
telldus_rawDeviceEventCallback(const char *data, int controllerId, int callbackId, void *pyfunc)
{
	PyObject * result; 
	PyGILState_STATE gstate;
	
	gstate = PyGILState_Ensure();

	// now call the Python callback function
	result = PyObject_CallFunction(pyfunc, "sll", data, controllerId, callbackId);

	if (result == NULL) {
		// something went wrong so print to stderr
		PyErr_Print();
	}

	// take care of reference handling
	Py_XDECREF(result);
	
	PyGILState_Release(gstate);
	
	return;
}

void
telldus_sensorEventCallback(const char *protocol, const char *model, int id, int dataType, const char *value, int timestamp, int callbackId, void *pyfunc)
{
	PyObject * result; 
	PyGILState_STATE gstate;
	
	gstate = PyGILState_Ensure();

	// now call the Python callback function
	result = PyObject_CallFunction(pyfunc, "ssllsll", protocol, model, id, dataType, value, timestamp, callbackId);

	if (result == NULL) {
		// something went wrong so print to stderr
		PyErr_Print();
	}

	// take care of reference handling
	Py_XDECREF(result);
	
	PyGILState_Release(gstate);
	
	return; 
}

static PyObject *
telldus_tdRegisterDeviceEvent(PyObject *self, PyObject *args)
{
	PyObject *func;

	if (!PyArg_ParseTuple(args, "O", &func));
		return NULL;
		
	// stick around till we need you
	Py_INCREF(func);
	
	return PyLong_FromLong((long) tdRegisterDeviceEvent((TDDeviceEvent) telldus_deviceEventCallback, &func));
}

static PyObject *
telldus_tdRegisterDeviceChangeEvent(PyObject *self, PyObject *args)
{
	PyObject *func;

	if (!PyArg_ParseTuple(args, "O", &func));
		return NULL;
		
	// stick around till we need you
	Py_INCREF(func);
			
	return PyLong_FromLong((long) tdRegisterDeviceChangeEvent((TDDeviceChangeEvent) telldus_deviceChangeEventCallback, &func));
}

static PyObject *
telldus_tdRegisterRawDeviceEvent(PyObject *self, PyObject *args)
{
	PyObject *func;

	if (!PyArg_ParseTuple(args, "O", &func));
		return NULL;
			
	// stick around till we need you
	Py_INCREF(func);
		
	return PyLong_FromLong((long) tdRegisterRawDeviceEvent((TDRawDeviceEvent) telldus_rawDeviceEventCallback, &func));
}

static PyObject *
telldus_tdRegisterSensorEvent(PyObject *self, PyObject *args)
{
	PyObject *func;

	if (!PyArg_ParseTuple(args, "O", &func));
		return NULL;
		
	// stick around till we need you
	Py_INCREF(func);
			
	return PyLong_FromLong((long) tdRegisterSensorEvent((TDSensorEvent) telldus_sensorEventCallback, &func));
}

static PyObject *
telldus_tdUnregisterCallback(PyObject *self, PyObject *args)
{
	long id;

	if (!PyArg_ParseTuple(args, "l", &id));
		return NULL;

	return PyBool_FromLong((long) tdUnregisterCallback(id));
}

static PyObject *
telldus_tdSensor(PyObject *self, PyObject *args)
{
	char protocol[DATA_LENGTH];
	char model[DATA_LENGTH];
	long sensorId = 0;
	long dataTypes = 0;
		
	long result;

   result = tdSensor(protocol, DATA_LENGTH, model, DATA_LENGTH, &sensorId, &dataTypes);
   
	if (result == TELLSTICK_SUCCESS)
	{
		return Py_BuildValue("ssll", protocol, model, sensorId, dataTypes);
	}
	else
	{
	   return PyBool_FromLong(result);
	}

}

static PyObject *
telldus_tdSensorValue(PyObject *self, PyObject *args)
{
	char protocol[DATA_LENGTH];
	char model[DATA_LENGTH];
	long sensorId = 0;
	long dataType = 0;
	char value[DATA_LENGTH];
	long timestamp = 0;

	PyObject *floatObj; 
	PyObject *timeTuple; 
	
	long result;

	PyObject *py_date;
	
	if (!PyArg_ParseTuple(args, "ssll", &protocol, &model, &sensorId, &dataType));
		return NULL;
	
	result = tdSensorValue(protocol, model, sensorId, dataType, &value, DATA_LENGTH, &timestamp);
	
	floatObj = PyFloat_FromDouble(timestamp); 
	timeTuple = Py_BuildValue("(O)", floatObj);
	Py_DECREF(floatObj);	
			
	py_date = PyDateTime_FromTimestamp(timeTuple);
	
	Py_DECREF(timeTuple);
	
	Py_INCREF(py_date);
	
	if (result == TELLSTICK_SUCCESS)
	{
		return Py_BuildValue("sO", value, py_date);
	}
	else
	{
	   return PyBool_FromLong(result);
	}
}

static PyMethodDef telldus_methods[] = {
	{"tdInit", (PyCFunction) telldus_tdInit, METH_NOARGS, "Initiate telldus."},
	{"tdClose", (PyCFunction) telldus_tdClose, METH_NOARGS, "Close telldus."},
	{"tdTurnOn", (PyCFunction) telldus_tdTurnOn, METH_VARARGS, "Turn on device."},
	{"tdTurnOff", (PyCFunction) telldus_tdTurnOff, METH_VARARGS, "Turn off device."},
	{"tdBell", (PyCFunction) telldus_tdBell, METH_VARARGS, "Bell device."},
	{"tdDim", (PyCFunction) telldus_tdDim, METH_VARARGS, "Dim device."},
	{"tdLearn", (PyCFunction) telldus_tdLearn, METH_VARARGS, "Learn device."},
	{"tdMethods", (PyCFunction) telldus_tdMethods, METH_VARARGS, "Methods comment."},
	{"tdLastSentCommand", (PyCFunction) telldus_tdLastSentCommand, METH_VARARGS, "LastSentCommand comment."},
	{"tdLastSentValue", (PyCFunction) telldus_tdLastSentValue, METH_VARARGS, "LastSentValue comment."},
	{"tdGetNumberOfDevices", (PyCFunction) telldus_tdGetNumberOfDevices, METH_VARARGS, "GetNumberOfDevices comment."},
	{"tdGetDeviceId", (PyCFunction) telldus_tdGetDeviceId, METH_VARARGS, "GetDeviceId comment."},
	{"tdGetDeviceType", (PyCFunction) telldus_tdGetDeviceType, METH_VARARGS, "GetDeviceType comment."},
	{"tdGetErrorString", (PyCFunction) telldus_tdGetErrorString, METH_VARARGS, "GetErrorString comment."},
	{"tdGetName", (PyCFunction) telldus_tdGetName, METH_VARARGS, "GetName comment."},
	{"tdSetName", (PyCFunction) telldus_tdSetName, METH_VARARGS, "SetName comment."},
	{"tdGetProtocol", (PyCFunction) telldus_tdGetProtocol, METH_VARARGS, "GetProtocol comment."},
	{"tdSetProtocol", (PyCFunction) telldus_tdSetProtocol, METH_VARARGS, "SetProtocol comment."},
	{"tdGetModel", (PyCFunction) telldus_tdGetModel, METH_VARARGS, "GetModel comment."},
	{"tdSetModel", (PyCFunction) telldus_tdSetModel, METH_VARARGS, "SetModel comment."},
	{"tdGetDeviceParameter", (PyCFunction) telldus_tdGetDeviceParameter, METH_VARARGS, "GetDeviceParameter comment."},
	{"tdSetDeviceParameter", (PyCFunction) telldus_tdSetDeviceParameter, METH_VARARGS, "SetDeviceParameter comment."},
	{"tdAddDevice", (PyCFunction) telldus_tdAddDevice, METH_NOARGS, "AddDevice comment."},
	{"tdRemoveDevice", (PyCFunction) telldus_tdRemoveDevice, METH_VARARGS, "RemoveDevice comment."},
	{"tdSendRawCommand", (PyCFunction) telldus_tdSendRawCommand, METH_VARARGS, "SendRawCommand comment."},

	{"tdRegisterDeviceEvent", (PyCFunction) telldus_tdRegisterDeviceEvent, METH_VARARGS, "RegisterDeviceEvent comment."},
	{"tdRegisterDeviceChangeEvent", (PyCFunction) telldus_tdRegisterDeviceChangeEvent, METH_VARARGS, "RegisterDeviceChangeEvent comment."},
	{"tdRegisterRawDeviceEvent", (PyCFunction) telldus_tdRegisterRawDeviceEvent, METH_VARARGS, "RegisterRawDeviceEvent comment."},
	{"tdRegisterSensorEvent", (PyCFunction) telldus_tdRegisterSensorEvent, METH_VARARGS, "RegisterSensorEvent comment."},
	{"tdUnregisterCallback", (PyCFunction) telldus_tdUnregisterCallback, METH_VARARGS, "UnregisterCallback comment."},
	{"tdSensor", (PyCFunction) telldus_tdSensor, METH_NOARGS, "Sensor comment."},
	{"tdSensorValue", (PyCFunction) telldus_tdSensorValue, METH_VARARGS, "SensorValue comment."},
	
	{NULL, NULL, 0, NULL}   /* sentinel */
};

void
inittelldus(void)
{
	PyObject *module;
	
	PyObject *TELLSTICK_TURNON_GLUE;
	PyObject *TELLSTICK_TURNOFF_GLUE;
	PyObject *TELLSTICK_BELL_GLUE;
	PyObject *TELLSTICK_TOGGLE_GLUE;
	PyObject *TELLSTICK_DIM_GLUE;
	PyObject *TELLSTICK_LEARN_GLUE;
	PyObject *TELLSTICK_SUCCESS_GLUE;
	PyObject *TELLSTICK_ERROR_NOT_FOUND_GLUE;
	PyObject *TELLSTICK_ERROR_PERMISSION_DENIED_GLUE;
	PyObject *TELLSTICK_ERROR_DEVICE_NOT_FOUND_GLUE;
	PyObject *TELLSTICK_ERROR_METHOD_NOT_SUPPORTED_GLUE;
	PyObject *TELLSTICK_ERROR_COMMUNICATION_GLUE;
	PyObject *TELLSTICK_ERROR_UNKNOWN_GLUE;
	PyObject *TELLSTICK_TYPE_DEVICE_GLUE;
	PyObject *TELLSTICK_TYPE_GROUP_GLUE;
	PyObject *TELLSTICK_TEMPERATURE_GLUE;
	PyObject *TELLSTICK_HUMIDITY_GLUE;
		
	/* Create the module and add the functions */

	PyEval_InitThreads();
	
	module = Py_InitModule("telldus", telldus_methods);
	
	TELLSTICK_TURNON_GLUE = PyLong_FromLong((long) TELLSTICK_TURNON);
	PyObject_SetAttrString(module, "TELLSTICK_TURNON", TELLSTICK_TURNON_GLUE);
	Py_DECREF(TELLSTICK_TURNON_GLUE);
		
	TELLSTICK_TURNOFF_GLUE = PyLong_FromLong((long) TELLSTICK_TURNOFF);
	PyObject_SetAttrString(module, "TELLSTICK_TURNOFF", TELLSTICK_TURNOFF_GLUE);
	Py_DECREF(TELLSTICK_TURNOFF_GLUE);
		
	TELLSTICK_BELL_GLUE = PyLong_FromLong((long) TELLSTICK_BELL);
	PyObject_SetAttrString(module, "TELLSTICK_BELL", TELLSTICK_BELL_GLUE);
	Py_DECREF(TELLSTICK_BELL_GLUE);
		
	TELLSTICK_TOGGLE_GLUE = PyLong_FromLong((long) TELLSTICK_TOGGLE);
	PyObject_SetAttrString(module, "TELLSTICK_TOGGLE", TELLSTICK_TOGGLE_GLUE);
	Py_DECREF(TELLSTICK_TOGGLE_GLUE);
		
	TELLSTICK_DIM_GLUE = PyLong_FromLong((long) TELLSTICK_DIM);
	PyObject_SetAttrString(module, "TELLSTICK_DIM", TELLSTICK_DIM_GLUE);
	Py_DECREF(TELLSTICK_DIM_GLUE);
		
	TELLSTICK_LEARN_GLUE = PyLong_FromLong((long) TELLSTICK_LEARN);
	PyObject_SetAttrString(module, "TELLSTICK_LEARN", TELLSTICK_LEARN_GLUE);
	Py_DECREF(TELLSTICK_LEARN_GLUE);
		
	TELLSTICK_SUCCESS_GLUE = PyLong_FromLong((long) TELLSTICK_SUCCESS);
	PyObject_SetAttrString(module, "TELLSTICK_SUCCESS", TELLSTICK_SUCCESS_GLUE);
	Py_DECREF(TELLSTICK_SUCCESS_GLUE);
		
	TELLSTICK_ERROR_NOT_FOUND_GLUE = PyLong_FromLong((long) TELLSTICK_ERROR_NOT_FOUND);
	PyObject_SetAttrString(module, "TELLSTICK_ERROR_NOT_FOUND", TELLSTICK_ERROR_NOT_FOUND_GLUE);
	Py_DECREF(TELLSTICK_ERROR_NOT_FOUND_GLUE);
		
	TELLSTICK_ERROR_PERMISSION_DENIED_GLUE = PyLong_FromLong((long) TELLSTICK_ERROR_PERMISSION_DENIED);
	PyObject_SetAttrString(module, "TELLSTICK_ERROR_PERMISSION_DENIED", TELLSTICK_ERROR_PERMISSION_DENIED_GLUE);
	Py_DECREF(TELLSTICK_ERROR_PERMISSION_DENIED_GLUE);
	
	TELLSTICK_ERROR_DEVICE_NOT_FOUND_GLUE = PyLong_FromLong((long) TELLSTICK_ERROR_DEVICE_NOT_FOUND);
	PyObject_SetAttrString(module, "TELLSTICK_ERROR_DEVICE_NOT_FOUND", TELLSTICK_ERROR_DEVICE_NOT_FOUND_GLUE);
	Py_DECREF(TELLSTICK_ERROR_DEVICE_NOT_FOUND_GLUE);
	
	TELLSTICK_ERROR_METHOD_NOT_SUPPORTED_GLUE = PyLong_FromLong((long) TELLSTICK_ERROR_METHOD_NOT_SUPPORTED);
	PyObject_SetAttrString(module, "TELLSTICK_ERROR_METHOD_NOT_SUPPORTED", TELLSTICK_ERROR_METHOD_NOT_SUPPORTED_GLUE);
	Py_DECREF(TELLSTICK_ERROR_METHOD_NOT_SUPPORTED_GLUE);   
	
	TELLSTICK_ERROR_COMMUNICATION_GLUE = PyLong_FromLong((long) TELLSTICK_ERROR_COMMUNICATION);
	PyObject_SetAttrString(module, "TELLSTICK_ERROR_COMMUNICATION", TELLSTICK_ERROR_COMMUNICATION_GLUE);
	Py_DECREF(TELLSTICK_ERROR_COMMUNICATION_GLUE);   
	
	TELLSTICK_ERROR_UNKNOWN_GLUE = PyLong_FromLong((long) TELLSTICK_ERROR_UNKNOWN);
	PyObject_SetAttrString(module, "TELLSTICK_ERROR_UNKNOWN", TELLSTICK_ERROR_UNKNOWN_GLUE);
	Py_DECREF(TELLSTICK_ERROR_UNKNOWN_GLUE);   

	TELLSTICK_TYPE_DEVICE_GLUE = PyLong_FromLong((long) TELLSTICK_TYPE_DEVICE);
	PyObject_SetAttrString(module, "TELLSTICK_TYPE_DEVICE", TELLSTICK_TYPE_DEVICE_GLUE);
	Py_DECREF(TELLSTICK_TYPE_DEVICE_GLUE);
	
	TELLSTICK_TYPE_GROUP_GLUE = PyLong_FromLong((long) TELLSTICK_TYPE_GROUP);
	PyObject_SetAttrString(module, "TELLSTICK_TYPE_GROUP", TELLSTICK_TYPE_GROUP_GLUE);
	Py_DECREF(TELLSTICK_TYPE_GROUP_GLUE);

	TELLSTICK_TEMPERATURE_GLUE = PyLong_FromLong((long) TELLSTICK_TEMPERATURE);
	PyObject_SetAttrString(module, "TELLSTICK_TEMPERATURE", TELLSTICK_TEMPERATURE_GLUE);
	Py_DECREF(TELLSTICK_TEMPERATURE_GLUE);
	
	TELLSTICK_HUMIDITY_GLUE = PyLong_FromLong((long) TELLSTICK_HUMIDITY);
	PyObject_SetAttrString(module, "TELLSTICK_HUMIDITY", TELLSTICK_HUMIDITY_GLUE);
	Py_DECREF(TELLSTICK_HUMIDITY_GLUE);

}
