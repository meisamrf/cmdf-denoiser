/*
Blockmatching cnn python wrapper
Meisam Rakhshanfar 2018
*/

#include <Python.h>

static PyObject *GenError;

#define BMCNN_LAYERS 7
class bmcnn
{
public:

	bool open(const char *filename);
	void predict(const float *img, float *denoised, int width, int height);
	void release();

private:
	float *weights[BMCNN_LAYERS];
	float *bias[BMCNN_LAYERS];
private:
	int dilation[BMCNN_LAYERS];
	int filterNum[BMCNN_LAYERS];
	int relu[BMCNN_LAYERS];

};

PyObject* py_open_bmcnn(PyObject *self, PyObject *args)
{
	char *filename;
	if (!PyArg_ParseTuple(args, "s", &filename)) {
		PyErr_SetString(GenError, "file name input error");
		return NULL;
	}
	bmcnn *net = new bmcnn;
	bool res = net->open(filename);
	if (!res) {
		PyErr_SetString(GenError, "cannot open weight file");
		return NULL;
	}

	long long *lret = (long long *)&net;
	return PyLong_FromLongLong(*lret);
}

PyObject* py_release_bmcnn(PyObject *self, PyObject *args)
{
	long long handle;
	if (!PyArg_ParseTuple(args, "L", &handle)) {
		PyErr_SetString(GenError, "input error");
		return NULL;
	}
	if (handle == 0) {
		PyErr_SetString(GenError, "handle error");
		return NULL;
	}
	bmcnn *net = (bmcnn *)handle;
	net->release();
	delete net;
	return PyLong_FromLong(0);
}

PyObject* py_predict_bmcnn(PyObject *self, PyObject *args)
{
	PyObject *arg1, *arg2;
	Py_buffer py_in, py_out;

	long long handle;
	if (!PyArg_ParseTuple(args, "LOO", &handle, &arg1, &arg2)) {
		PyErr_SetString(GenError, "input error");
		return NULL;
	}
	if (handle == 0) {
		PyErr_SetString(GenError, "handle error");
		return NULL;
	}
	if (PyObject_GetBuffer(arg1, &py_in, PyBUF_FULL) < 0 ||
		PyObject_GetBuffer(arg2, &py_out, PyBUF_FULL) < 0)
		return NULL;

	if (py_in.itemsize != 4 || py_in.ndim != 2) {
		PyErr_SetString(GenError, "The input do not have the expected size or shape");
		return NULL;
	}

	if (py_out.itemsize != 4 || py_out.ndim != 2 || py_out.shape[0] != py_in.shape[0] ||
		py_out.shape[1] != py_in.shape[1]) {
		PyErr_SetString(GenError, "The output do not have the expected size or shape");
		return NULL;
	}

	bmcnn *net = (bmcnn *)handle;
	net->predict((float *)py_in.buf, (float *)py_out.buf, (int)py_in.shape[1], (int)py_in.shape[0]);

	PyBuffer_Release(&py_in);
	PyBuffer_Release(&py_out);
	return PyLong_FromLong(0);
}


PyMethodDef bmcnnMethods[] = {
	{ "open", (PyCFunction)py_open_bmcnn, METH_VARARGS, "Open (weight_file)" },
	{ "predict", (PyCFunction)py_predict_bmcnn, METH_VARARGS, "predict (image_in, image_out)" },
	{ "release", (PyCFunction)py_release_bmcnn, METH_VARARGS, "release" },
	{ NULL, NULL, 0, NULL }
};


static struct PyModuleDef bmcnnmodule = {
	PyModuleDef_HEAD_INIT,
	"bmcnn",   /* name of module */
	"bmcnn Module C++", /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
			  or -1 if the module keeps state in global variables. */
	bmcnnMethods
};

PyMODINIT_FUNC
PyInit_bmcnn(void)
{
	PyObject *m = PyModule_Create(&bmcnnmodule);

	if (m == NULL)
		return NULL;

	GenError = PyErr_NewException("bmcnn.error", NULL, NULL);
	Py_INCREF(GenError);
	PyModule_AddObject(m, "error", GenError);

	return m;

}
