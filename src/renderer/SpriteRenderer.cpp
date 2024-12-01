#include "renderer/SpriteRenderer.h"

//#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "renderer/rData.h"
#include "renderer/Renderer.h"
#include <typeinfo>
#include "ecs/Transform.h"

namespace Pontilus
{
    namespace Renderer
    {
        
        void SpriteRenderer::init(Renderer::Texture t, glm::vec4 color)
        {
            this->tex = t;
            this->color = color;
        }

        using namespace Renderer; // fight me
        void SpriteRenderer::setVisible(bool v)
        {
            this->visible = v;
        }

        int SpriteRenderer::update(double) {
            auto quadTarget = Renderer::RendererController::get().getTarget(1);

            return this->toRData(*std::get<0>(quadTarget));
        }

        int SpriteRenderer::toRData(rData &r)
        {
            using namespace Pontilus::ECS;
            if (!this->visible)
                return 0;

            uint32_t stride = r.dataOffset;

            Transform t = *dynamic_cast<Transform *>(parent->getComponent(typeid(Transform)));

            constexpr auto ident = glm::identity<glm::mat4>();

            glm::mat4 rotation = rotate(ident, t.rot.x, glm::vec3{1.0f, 0.0f, 0.0f});
            rotation = rotate(rotation, t.rot.y, glm::vec3{0.0f, 1.0f, 0.0f});
            rotation = rotate(rotation, t.rot.z, glm::vec3{0.0f, 0.0f, 1.0f});
            glm::mat4 translation = translate(rotation, t.pos);
            for (int i = 0; i < 4; i++)
            {
                glm::vec3 orientation;
                switch (i)
                {
                    case 0: orientation = {1.0f * t.whd.x, 1.0f * t.whd.y, 0.0f}; break;
                    case 1: orientation = {0.0f * t.whd.x, 1.0f * t.whd.y, 0.0f}; break;
                    case 2: orientation = {0.0f * t.whd.x, 0.0f * t.whd.y, 0.0f}; break;
                    case 3: orientation = {1.0f * t.whd.x, 0.0f * t.whd.y, 0.0f}; break;
                    default: { }
                }


                rData::off_len result = r.getAttribMetaData(PONT_POS);
                if (result.second >= 4 * sizeof(float))
                {
                    glm::vec4 t_orient = translation * rotation * glm::vec4(orientation - glm::vec3(0.5f * t.whd.x, 0.5f * t.whd.y, 0.0f), 1.0f);

                    memcpy(static_cast<char *>(r.data) + result.first + stride, value_ptr(t_orient), 4 * sizeof(float));
                    //for (int j = 0; j < 3; j++)
                    //{
                    //    ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&t.pos)[j];
                    //}
                }
                
                result = r.getAttribMetaData(PONT_COLOR);
                if (result.second >= 4 * sizeof(float))
                {
                    for (int j = 0; j < 4; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&this->color)[j];
                    }             
                }

                result = r.getAttribMetaData(PONT_TEXCOORD);
                
                if (result.second >= 2 * sizeof(float))
                {
                    orientation.x /= t.whd.x;
                    orientation.y /= t.whd.y;
                    for (int j = 0; j < 2; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = this->tex.source == nullptr ? 0.0 : this->tex.texCoords[j + i * 2];
                    }
                }

                result = r.getAttribMetaData(PONT_TEXID);
                if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                {
                    if (this->tex.source == nullptr)
                    {
                        *(float *)((char *)r.data + result.first + stride) = 0.0f;
                    }
                    else
                    {
                        *(float *)((char *)r.data + result.first + stride) = this->tex.source == nullptr ? 0.0 : this->tex.source->id();
                    }
                }
                stride += r.getLayoutLen();
            }

            r.isDirty = true;
            
            r.dataOffset =  stride;
            
            return 0;
        }
    }
}

