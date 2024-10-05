#pragma once
#include "glm/glm.hpp"
#include <bitset>

/**
 * Math types
 */
typedef glm::vec2 Vector2;

/**
 * ECS types
 *
 * TODO: rather than having a maximum number of components restricted by the size of
 * a single data type, could refactor to use data structures as signatures instead.
 * That way we don't have a restriction on the number of components we can write.
 */
typedef std::bitset<CoreStatics::MaxNumComponents> Signature;