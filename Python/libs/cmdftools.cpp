// cmdf python wrapper 
// by Meisam Rakhshanfar

#include <Python.h>

static PyObject *GenError;


class cmdf
{
public:
	static int filter1(float * img_out, const float * img, int img_row, int img_col, float nv);
	static int filter2(float * y2, float * y1, const float * img, int img_row, int img_col, float nv);
};


PyObject* py_filter1(PyObject *self, PyObject *args)
{
	PyObject *arg1, *arg2;
	Py_buffer b_imgin, b_imgout;
	float nv, sigma;

	if (!PyArg_ParseTuple(args, "OOf", &arg1, &arg2, &sigma))
		return NULL;

	if (PyObject_GetBuffer(arg1, &b_imgin, PyBUF_FULL) < 0)
		return NULL;
	
	if (PyObject_GetBuffer(arg2, &b_imgout, PyBUF_FULL) < 0)
		return NULL;

	if (b_imgin.itemsize != 4 || b_imgout.itemsize != 4) {
		PyErr_SetString(GenError, "data type error");
		return NULL;
	}
	if (b_imgin.ndim != 2 || b_imgout.ndim != 2) {
		PyErr_SetString(GenError, "dimension type error");
		return NULL;
	}
	
	int img_row = (int)b_imgin.shape[0];
	int img_col = (int)b_imgin.shape[1];

	int img_row_o = (int)b_imgout.shape[0];
	int img_col_o = (int)b_imgout.shape[1];

	if (img_row != img_row_o || img_col != img_col_o) {
		PyErr_SetString(GenError, "Output dimension error .\n");
		return NULL;
	}

	if (img_row % 8 != 0 || img_col % 8 != 0) {
		PyErr_SetString(GenError, "Parameter error .\n");
		return NULL;
	}

	if (img_row > 3840 || img_col > 3840 || img_row < 64 || img_col < 64) {
		PyErr_SetString(GenError, "Image size error .\n");
		return NULL;
	}
	
	nv = sigma*sigma*0.75f;
	int results = cmdf::filter1((float *)b_imgout.buf, (float *)b_imgin.buf, img_col, img_row, nv);

	PyObject* res = PyLong_FromLong(results);
	PyBuffer_Release(&b_imgin);
	PyBuffer_Release(&b_imgout);

	return res;
}

PyObject* py_filter2(PyObject *self, PyObject *args)
{
	PyObject *arg1, *arg2, *arg3;
	Py_buffer b_imgin, b_imgout1, b_imgout2;
	float sigma;

	if (!PyArg_ParseTuple(args, "OOOf", &arg1, &arg2, &arg3, &sigma))
		return NULL;

	if (PyObject_GetBuffer(arg1, &b_imgin, PyBUF_FULL) < 0)
		return NULL;

	if (PyObject_GetBuffer(arg2, &b_imgout1, PyBUF_FULL) < 0)
		return NULL;

	if (PyObject_GetBuffer(arg3, &b_imgout2, PyBUF_FULL) < 0)
		return NULL;

	if (b_imgin.itemsize != 4 || b_imgout1.itemsize != 4 || b_imgout2.itemsize != 4) {
		PyErr_SetString(GenError, "data type error");
		return NULL;
	}
	if (b_imgin.ndim != 2 || b_imgout1.ndim != 2 || b_imgout2.ndim != 2) {
		PyErr_SetString(GenError, "dimension type error");
		return NULL;
	}


	int img_row = (int)b_imgin.shape[0];
	int img_col = (int)b_imgin.shape[1];

	int img_row_o1 = (int)b_imgout1.shape[0];
	int img_col_o1 = (int)b_imgout1.shape[1];
	
	int img_row_o2 = (int)b_imgout2.shape[0];
	int img_col_o2 = (int)b_imgout2.shape[1];

	if (img_row != img_row_o1 || img_row != img_row_o2 || img_col != img_col_o1 || img_col != img_col_o2) {
		PyErr_SetString(GenError, "Output dimension error .\n");
		return NULL;
	}

	if (img_row % 8 != 0 || img_col % 8 != 0) {
		PyErr_SetString(GenError, "Parameter error .\n");
		return NULL;
	}

	if (img_row > 2160 || img_col > 3840 || img_row < 64 || img_col < 64) {
		PyErr_SetString(GenError, "Image size error .\n");
		return NULL;
	}

	int results = cmdf::filter2((float *)b_imgout2.buf, (float *)b_imgout1.buf, (float *)b_imgin.buf, img_col, img_row, sigma*sigma);

	PyObject* res = PyLong_FromLong(results);
	PyBuffer_Release(&b_imgin);
	PyBuffer_Release(&b_imgout1);
	PyBuffer_Release(&b_imgout2);

	return res;
}

PyMethodDef CmdfMethods[] = {
	{ "filter1",(PyCFunction)py_filter1,METH_VARARGS,"Cascaded Multi-Domain Image Filter Non-Local" },
	{ "filter2",(PyCFunction)py_filter2,METH_VARARGS,"Cascaded Multi-Domain Image Filter Local Dual-Domain" },
	{ NULL, NULL, 0, NULL }
};


static struct PyModuleDef cmdfmodule = {
	PyModuleDef_HEAD_INIT,
	"cmdftools",
	"cmdf Module C++",
	-1,			  
	CmdfMethods
};

PyMODINIT_FUNC
PyInit_cmdftools(void)
{
	PyObject *m = PyModule_Create(&cmdfmodule);

	if (m == NULL)
		return NULL;

	GenError = PyErr_NewException("cmdftools.error", NULL, NULL);
	Py_INCREF(GenError);
	PyModule_AddObject(m, "error", GenError);

	return m;

}
