#pragma once

// Publishes the local character to the standalone CharacterDashboard.exe.
// Events are NOT published here — CharacterDashboard.exe queries GameServer.
// Calls are deliberately inexpensive and safe to make from the game loop.
void InitializeCharacterDashboard();
void UpdateCharacterDashboard();
void ShutdownCharacterDashboard();
