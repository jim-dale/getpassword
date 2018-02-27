#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <windows.h>
#include <wincred.h>

#include <stdio.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "AppCommandType.h"
#include "Utility.h"
#include "AppMetadata.h"
#include "SimpleGenericCredential.h"
#include "CredentialManager.h"
#include "AppCommand.h"
#include "AppContext.h"
#include "CommandLineManager.h"
