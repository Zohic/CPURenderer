#include "TriangleWrap.h"
using namespace cpuRenderBase;


TriangleWrap::TriangleWrap(VertexData* v1) : _vert(v1) {

}
TriangleWrap::TriangleWrap(const VertexData* v1) : _vert(const_cast<VertexData*>(v1)) {

}

TriangleWrap::TriangleWrap(TriangleWrap&& old) noexcept {
	this->_vert = old._vert;
	old._vert = nullptr;
}
TriangleWrap& TriangleWrap::operator=(TriangleWrap&& old) noexcept {
	this->_vert = old._vert;
	old._vert = nullptr;
	return *this;
}

bool TriangleWrap::operator ==(const TriangleWrap& other) const {
	return _vert == other._vert;
}

VertexData& TriangleWrap::operator[](const size_t n) {
#ifdef CPUREN_DEBUG
	printf("returning vData with index %zu: (%f, %f, %f, %f)\n", n,
		_vert[n].GetPos().x(), _vert[n].GetPos().y(), _vert[n].GetPos().z(), _vert[n].GetPos().w());
#endif // CPUREN_DEBUG

	if (n > 2 || n < 0)
		throw std::logic_error("trignale out of bounds!");
	return *(_vert + n);
}

const VertexData& TriangleWrap::operator[](size_t n) const {
#ifdef CPUREN_DEBUG
	printf("returning const vData with index %zu: (%f, %f, %f, %f)\n", n,
		_vert[n].GetPos().x(), _vert[n].GetPos().y(), _vert[n].GetPos().z(), _vert[n].GetPos().w());
#endif // CPUREN_DEBUG
	if (n > 2 || n < 0)
		throw std::logic_error("trignale out of bounds!");
	return *(_vert + n);
}


TriangleWrap::~TriangleWrap() noexcept(false) {
#ifdef _DEBUG
	assert(_vert == nullptr);
#endif
}
