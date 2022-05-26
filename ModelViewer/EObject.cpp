#include "EObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void EObject::OnUpdate()
{

}

void EObject::OnRender()
{
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, transform.position);
    modelMatrix = glm::scale(modelMatrix, transform.scale);

    glm::mat4 rotateMatrix;

    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

    modelMatrix *= rotateMatrix;


}
