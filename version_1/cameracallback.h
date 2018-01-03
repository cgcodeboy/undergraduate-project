#ifndef CAMERACALLBACK
#define CAMERACALLBACK

#include <osg/NodeCallback>
#include <osg/NodeVisitor>
#include <osgUtil/CullVisitor>
#include <osg/MatrixTransform>


class CameraTrackCallback: public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if( nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR )
        {
            osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>(nv);
            osg::Vec3f centre,up,eye;

            cv->getRenderStage()->getCamera()->getViewMatrixAsLookAt(eye,centre,up);

            osg::MatrixTransform* mt = static_cast<osg::MatrixTransform*>(node);
            mt->setMatrix( osg::Matrix::translate( eye.x(), eye.y(), mt->getMatrix().getTrans().z() ) );
        }

        traverse(node, nv);
    }
};

#endif // CAMERACALLBACK

