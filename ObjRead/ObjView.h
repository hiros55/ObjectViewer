class Obj {
public:
	struct Vector3f;
	struct ObjDate;
	Obj () {};
	~Obj(){};
	ObjDate* Load(const char* FileName);
	void RenderObj(Obj::ObjDate* objectDate);
};