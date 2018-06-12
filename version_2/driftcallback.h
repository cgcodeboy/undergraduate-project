#ifndef DRIFTCALLBACK
#define DRIFTCALLBACK

#include <osg/NodeCallback>
#include <osgOcean/OceanScene>
#include <osg/NodeVisitor>
#include <osg/Matrix>

#include <QDebug>
#include <QString>

class DriftCallback :public osg::NodeCallback{
public:
    DriftCallback(osgOcean::OceanScene * scene,QString type){
        _scene = scene;
        _type = type;
    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv){
        if(nv->getVisitorType() ==osg::NodeVisitor::UPDATE_VISITOR)
        {
            osg::MatrixTransform* mat = dynamic_cast<osg::MatrixTransform*>(node);
            if(mat){
                osg::Matrix  matrix = osg::computeLocalToWorld(nv->getNodePath());
                osg::Vec3d pos = matrix.getTrans();

                osg::Vec3f normal;
                float height = _scene->getOceanSurfaceHeightAt(pos[0],pos[1],&normal);

                float scale;
                if(_type == "driftor"){
                    scale = 0.2;
                }
                else{
                    scale = 0.001;
                }
                mat->setMatrix(osg::Matrix::scale(scale,scale,scale)*osg::Matrix::translate(pos[0],pos[1],height));

            }
        }
    }
private:
    osgOcean::OceanScene * _scene;

    QString _type;
};

#endif // DRIFTCALLBACK

