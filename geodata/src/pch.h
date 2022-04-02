#pragma once

#include <utils/Assert.h>
#include <utils/ExtractionHelpers.h>
#include <utils/Log.h>

#include <geometry/Box.h>
#include <geometry/Sphere.h>
#include <geometry/Transformation.h>
#include <geometry/Triangle.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Recast.h"

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
