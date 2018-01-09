#ifndef DRIFTCALLBACK
#define DRIFTCALLBACK

#include <osg/NodeCallback>
#include <osgOcean/OceanScene>
#include <osg/NodeVisitor>
#include <osg/Matrix>

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

                mat->setMatrix(osg::Matrix::scale(0.005,0.005,0.005)*osg::Matrix::translate(pos[0],pos[1],height));

                //                static float a = 0;
                //                matrix.makeTranslate(osg::Vec3(pos[0]+cosf(a),pos[1]+sinf(a),height));

                //                osg::Matrixf rot;
                //                rot.makeIdentity();
                //                rot.makeRotate(normal.x(),osg::X_AXIS,
                //                               normal.y(),osg::Y_AXIS,
                //                               a,osg::Z_AXIS);

                //                a+=0.005;
                //                matrix = rot * matrix;
                //                mat->setMatrix(matrix);

            }
        }
    }
private:
    osgOcean::OceanScene * _scene;
};

#endif // DRIFTCALLBACK

