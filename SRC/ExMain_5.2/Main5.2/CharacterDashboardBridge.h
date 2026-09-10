#pragma once

// Publishes the local character to the standalone CharacterDashboard.exe.
// Calls are deliberately inexpensive and safe to make from the game loop.
void InitializeCharacterDashboard();
void UpdateCharacterDashboard();
void ShutdownCharacterDashboard();
