#ifndef DRIFTCALLBACK
#define DRIFTCALLBACK

#include <osg/NodeCallback>
#include <osgOcean/OceanScene>
#include <osg/NodeVisitor>
#include <osg/Matrix>

#include <QDebug>

class DriftCallback :public osg::NodeCallback{
public:
    DriftCallback(osgOcean::OceanScene * scene){
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

                osg::Vec3f normal;
                float height = _scene->getOceanSurfaceHeightAt(pos[0],pos[1],&normal);

                qDebug()<<height;

                mat->setMatrix(osg::Matrix::scale(0.005,0.005,0.005)*osg::Matrix::translate(pos[0],pos[1],height));

            }
        }
    }
private:
    osgOcean::OceanScene * _scene;

};

#endif // DRIFTCALLBACK

