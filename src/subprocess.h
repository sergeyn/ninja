// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef NINJA_SUBPROCESS_H_
#define NINJA_SUBPROCESS_H_

#include <string>
#include <vector>
#include <queue>

#ifdef WIN32
#include "win32port.h"
#endif

using namespace std;

struct SubprocessSet;

/// Subprocess wraps a single async subprocess.  It is entirely
/// passive: it expects the caller to notify it when its fds are ready
/// for reading, as well as call Finish() to reap the child once done()
/// is true.
struct Subprocess {
  Subprocess();
  Subprocess(SubprocessSet* set_we_are_run_on);
  ~Subprocess();
  bool Start(const string& command);
  void OnFDReady(int fd);
  /// Returns true on successful process exit.
  bool Finish();

  bool done() const {
#ifdef WIN32
    return stdout_.fd_ == NULL && stderr_.fd_ == NULL;
#else
    return stdout_.fd_ == -1 && stderr_.fd_ == -1;
#endif
  }

  struct Stream {
    Stream();
    ~Stream();
#ifdef WIN32
    void ProcessInput(DWORD numbytesread);
    int  state_; //0 - not connected, 1 - reading
    HANDLE fd_; // NULL when in 'accepting connection' state
    OVERLAPPED overlapped_;
    char overlappedbuf_[4<<10];
#else
    int fd_;
#endif
    string buf_;
  };
  Stream stdout_, stderr_;

#ifdef WIN32
  HANDLE pid_;
#else
  pid_t pid_;
#endif

  SubprocessSet* procset_;
};

/// SubprocessSet runs a poll() loop around a set of Subprocesses.
/// DoWork() waits for any state change in subprocesses; finished_
/// is a queue of subprocesses as they finish.
struct SubprocessSet {
  void Add(Subprocess* subprocess);
  void DoWork();
  Subprocess* NextFinished();

  vector<Subprocess*> running_;
  queue<Subprocess*> finished_;
};

#endif // NINJA_SUBPROCESS_H_
