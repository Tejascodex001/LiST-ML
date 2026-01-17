# System Anomaly Detector (C++)

A low-level, real-time system telemetry collector and statistical anomaly detection pipeline built in **modern C++**.  
The project focuses on **correct signal engineering, concurrency safety, and explainable anomaly scoring**.

---

## Overview

This system continuously samples OS-level metrics (CPU and memory), computes rolling statistics, and assigns anomaly scores in near real time.  
The output is a clean, ML-ready CSV time series.

**Primary goals**
- Accurate metric extraction from `/proc`
- Deterministic sampling
- Thread-safe data flow
- Explainable statistical anomaly detection
- Correct shutdown and data persistence

---

## Architecture
```
┌────────────┐
│ /proc/*    │
│ (Linux)    │
└─────┬──────┘
      │
      ▼
┌────────────┐
│ ProcReader │  → raw CPU & memory snapshots
└─────┬──────┘
      │
      ▼
┌────────────┐
│ Sampler    │  → usage %, rolling stats, z-scores
└─────┬──────┘
      │
      ▼
┌────────────┐
│ SampleQueue│  → thread-safe queue
└─────┬──────┘
      │
      ▼
┌────────────┐
│ Writer     │  → CSV persistence
└────────────┘
```


---

## Features

### Metrics Collected
- CPU usage (derived from jiffy deltas in `/proc/stat`)
- Memory usage (derived from `MemTotal` and `MemAvailable` in `/proc/meminfo`)

### Statistical Processing
- Fixed-size rolling window
- Rolling mean
- Rolling standard deviation
- Z-scores per metric
- Composite anomaly score:
    anomaly_score = |cpu_z| + |mem_z|


### State Classification
Each sample is labeled as:
- `NORMAL`
- `WARNING`
- `ANOMALY`

Based on anomaly score thresholds.

---

## Threading Model

- **Sampler (main thread)**
  - Periodically samples metrics (1 second)
  - Computes rolling statistics
  - Pushes enriched samples to a queue

- **Writer thread**
  - Consumes samples from the queue
  - Writes to CSV without blocking sampling

### Synchronization Primitives Used
- `std::mutex`
- `std::condition_variable`
- `std::atomic<bool>` for shutdown signaling

Supports clean shutdown via `SIGINT` (Ctrl+C).

---

## Output Format

CSV output (written in real time):

```csv
timestamp,cpu_usage,memory_usage,cpu_mean,mem_mean,cpu_std,mem_std,cpu_z,mem_z,anomaly_score,state
1768482792,0.0068836,0.342286,0.0227742,0.345726,0.0210366,0.00244281,-0.755376,-1.40843,2.16381,NORMAL
```

The format is directly usable for:
- Offline ML training
- Streaming pipelines
- Visualization dashboards

## Build Instructions

### Requirements:
- Linux (for `proc/`)
- C++ 17 or newer
- CMake

### Build:
```
cmake -S . -B build
cmake --build build
```
### Run:
```
./build/collector/collector
```
### Stop:
```
ctrl+c
```

## Design Implications and decisions

- Rolling window is O(n)
    - Implemented for clarity and correctness.
    - Can be optimized using incremental statistics
- Warm-up period (first 30 seconds)
    - Statistics are computed only after the rolling window is full.
    - Early samples are intentionally skipped.

## Project Status

- Completed (Core Pipeline)
- Implemented:
    - Metric collection
    - Sampling pipeline
    - Rolling statistics
    - Z-score based anomaly Labeling
    - Thread-safe producer–consumer design
    - Clean shutdown
    - Persistent CSV output

## Possible Extensions

- Incremental rolling statistics (O(1))
- State stability / hysteresis
- Online ML inference
- Prometheus / Kafka exporters
- Visualization (Grafana, matplotlib)

## Notes I kept on referring for clarity and to avoid confusion:

NOTE: There are no comments in the code, plz refer readme or this documentation. 

Things I learnt from this project: 

- `std::chrono` and `std::chrono_literals` :- function used for time related operations. I have used `sleep_for(time_value)` to wait before executing a process. I have also used `chrono::system_clock::to_time_t(chrono::system_clock::now())` to get the current time which is based on the clock.
- .hpp :- These are the files used to declare headers and to link multiple files and to execute at once.
- Struct :- struct is used to define custom data types which have inbuilt data types.
        - Ex:
  ```
            struct sample{ 
                std::time_t timeStamp;
                double cpu_usage; 
              };
  ```

 - Proc/stat :- Stat is a file in proc which keeps info on cpu and other metrics. This project mainly revolves around it. I took the raw metrics from it and calculated the cpu use for each second (will add other metrics later).
 - `std::fstream` and `std::sstream` :- they give functions such as ifstream and `istringstream` which reads the file from the given path and streams the string into variables respectively.
 - `std::getline()` :- it is used to get a line from the file, say if a file has 100 lines, if we call `getline(filename, line)` where line is a variable of dtype string, it will only put the 1st line of the file into the line variable.
 - Cmake :- cmake is used to make it easier to compile multiple files into single build and run it once. It made me learn why it is industry standard and how to only specify required files in the build and how to link the .hpp files along with telling the IDE/code editor what is correct or wrong.
 - Class, objects and constructors :- refer google bro.
 - Writing to csv :- we need to use std::ofstream variable to write something into a file (it is what I learnt, I still need to learn about other methods if present). It is written to the file (file can be opened by `file.open())` using `file << “hello”<< “\n”;`.
 - `Std::seekp(position)` :-  move the cursor to the position.
             - Examples:
               - `std::seekp(0,. Std::ios::beg)` -> moves to beginning.
               - `Std::seekp(0, std::ios::end)` -> moves to end.
               - `Std::seekp(n)` -> moves to byte n.
  
 - `Std::tellp()` :- tells at what byte offset would the next write occur or returns current write position (in bytes). Example: `file.tellp()` -> returns the current write position in the file.
 - `Std::ios::<operation>` :- ios is something that you would get doubt on seeing above seekp and tellp. ios a collection of operations/switches that tell a stream how to behave. It is a named constant(bit-flag).
                 - Example:
                     - `ios::in`-> open for reading.
                     - `Ios::out` -> open for writing.
                     - `Ios::app` -> open for append etc.
You can ask chatgpt or google for further examples and ther use cases. In the code you can see I have opened the file in append mode, but under the hood there are two operations going on. `Ios::out` -> is called always and `ios::app` is what i called explicitly. These two operations are bitwise OR’ed together. That means the file is open on the condition to be written and appended. If the file exists, it appends, if not it creates and then writes.
 - Queue :- Implemented queue (can be used by std::queue and including it before using it). Queue is needed as we need to decouple the sampler and writer, i don't want sampler to lag due to writer and produce inconsistent values.
 - `Std::ref` :- used to pass a value by reference.
 - Mutex :- Used to protect shared data (in this case queue), only one thread can access it at one time. It gaurds the data. `Lock_guard` and `unique_lock` are used to lock the mutex. Rule is that `unqiue_lock` requires `condition_variable` (google it; this is not if, if else, else ifs).
 - Conditional_variable :- they are used to check some conditions and releases mutex based on the condition. Else it sleeps the mutex. This is checked every time. 

  

