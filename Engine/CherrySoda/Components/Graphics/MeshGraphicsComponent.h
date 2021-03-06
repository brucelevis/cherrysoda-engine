#ifndef _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_

#include <CherrySoda/Components/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Mesh.h>

namespace cherrysoda {

template <class VERTEX_T>
class MeshGraphicsComponent : public cherrysoda::GraphicsComponent
{
public:
	// TODO: It needs some workaround here for the template name
	typedef VERTEX_T VertexType;
	CHERRYSODA_DECLARE_COMPONENT(MeshGraphicsComponent<VertexType>, GraphicsComponent);

	MeshGraphicsComponent(bool active) : base(active) {}

	void Render() override
	{
		cherrysoda::Graphics::SetTransformMatrix(GetTransformMatrix());
		cherrysoda::Graphics::SubmitMesh(GetMesh());
	}

	cherrysoda::Mesh<VERTEX_T>* GetMesh() { return &m_mesh; }

private:
	cherrysoda::Mesh<VERTEX_T> m_mesh;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_MESHGRAPHICSCOMPONENT_H_
