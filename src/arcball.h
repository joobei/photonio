#pragma warning(disable: 4819)

#ifndef ARCBALL_H
#define ARCBALL_H

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "GL/glew.h"

using glm::vec2;
using glm::vec3;
using glm::quat;

//assuming IEEE-754(GLfloat), which i believe has max precision of 7 bits
# define Epsilon 1.0e-5

    typedef class ArcBall_t
    {
        protected:
            inline
            void _mapToSphere(const vec2* NewPt, vec3* NewVec) const;

        public:
            //Create/Destroy
                    ArcBall_t(GLfloat NewWidth, GLfloat NewHeight);
                   ~ArcBall_t() { /* nothing to do */ };

            //Set new bounds
            inline
            void    setBounds(GLfloat NewWidth, GLfloat NewHeight)
            {
                assert((NewWidth > 1.0f) && (NewHeight > 1.0f));

                //Set adjustment factor for width/height
                this->AdjustWidth  = 1.0f / ((NewWidth  - 1.0f) * 0.5f);
                this->AdjustHeight = 1.0f / ((NewHeight - 1.0f) * 0.5f);
            }

            //Mouse down
            void    click(const vec2* NewPt);

            //Mouse drag, calculate rotation
            void    drag(const vec2* NewPt, quat* NewRot);

        protected:
            vec3   StVec;          //Saved click vector
            vec3   EnVec;          //Saved drag vector
            GLfloat     AdjustWidth;    //Mouse bounds width
            GLfloat     AdjustHeight;   //Mouse bounds height

    } ArcBallT;

#endif

