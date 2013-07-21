#pragma once

#include <vector>

enum MovementFlag {
   kMoveForward,
   kMoveBack,
   kMoveLeft,
   kMoveRight,
   kMoveUp,
   kMoveDown,
   kRotateLeft,
   kRotateRight,
   kNumMovementFlags
};

class MovementFlags
{
public:
   MovementFlags() {}
   void SetFlag( MovementFlag flag ) { flags[flag] = true; }
   void ClearFlag( MovementFlag flag ) { flags[flag] = false; }
   bool Flag( MovementFlag flag ) const { return flags[flag]; }

   std::vector<bool> flags = std::vector<bool>( kNumMovementFlags, false );
};
