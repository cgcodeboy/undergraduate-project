#ifndef AIRPLANECALLBACK
#define AIRPLANECALLBACK

#include <osg/NodeCallback>
#include <osgOcean/OceanScene>
#include <osg/NodeVisitor>
#include <osg/Matrix>

class AirplaneCallback :public osg::NodeCallback{
public:
    AirplaneCallback(osgOcean::OceanScene * scene){
        _scene = scene;
    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv){
        if(nv->getVisitorType() ==osg::NodeVisitor::UPDATE_VISITOR)
        {
            osg::MatrixTransform* mat = dynamic_cast<osg::MatrixTransform*>(node);
            if(mat){
                osg::Matrix  matrix = osg::computeLocalToWorld(nv->getNodePath());
                osg::Vec3d pos = matrix.getTrans();

                float translate_offset = 10;

                mat->setMatrix(osg::Matrix::scale(0.005,0.005,0.005)*osg::Matrix::translate(pos[0],pos[1]+translate_offset,100));

            }
        }
    }
private:
    osgOcean::OceanScene * _scene;
};

#endif // AIRPLANECALLBACK

