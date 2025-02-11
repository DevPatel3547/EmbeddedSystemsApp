# Embedded System Monitoring with OpenBMC & Yocto

Welcome to the Embedded System Monitoring project! This repository contains a proof-of-concept application that monitors system metrics (CPU usage, memory usage, and power usage) on embedded devices. The project integrates multiple technologies including a multi-threaded C++ daemon, a Python Flask REST API, Yocto for custom Linux image builds, Jenkins for CI/CD, and Puppet for deployment automation.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Directory Structure](#directory-structure)
- [Prerequisites](#prerequisites)
- [Installation and Usage](#installation-and-usage)
  - [Building the C++ Daemon](#building-the-c-daemon)
  - [Running the Flask API](#running-the-flask-api)
  - [Starting All Services](#starting-all-services)
- [Yocto Integration](#yocto-integration)
- [Jenkins CI/CD Pipeline](#jenkins-cicd-pipeline)
- [Puppet Deployment](#puppet-deployment)
- [Contributing](#contributing)
- [License](#license)

## Overview

This project demonstrates how to build an embedded system monitoring solution using:

- **Multi-threaded C++ Daemon**: Collects CPU, memory, and power metrics every second.
- **Python Flask REST API**: Serves the collected metrics via a `/metrics` endpoint.
- **Yocto Project Recipe**: Packages the daemon and Flask application into a custom Linux image.
- **Jenkins Pipeline**: Automates building, testing, and deploying the project.
- **Puppet Manifest**: Provides automated deployment and service management on target devices.

## Features

- **Real-time Monitoring**: Gathers system health data (CPU, memory, power) every second.
- **Multi-threading**: Utilizes C++ threads with proper mutex locks for safe data sharing.
- **REST API**: Exposes system metrics in JSON format using a Flask server.
- **Custom Linux Build**: Uses Yocto to create a minimal Linux image containing the application.
- **CI/CD Pipeline**: A `Jenkinsfile` is included to automate building and deployment.
- **Deployment Automation**: Puppet scripts to deploy and manage the `system_monitor` service.

## Prerequisites

Before getting started, ensure you have the following installed:

- **Development Tools**:
  - A C++ compiler (e.g., `g++` with C++11 support)
  - GNU Make (or an equivalent build tool)
- **Python 3**: with Flask (see the `flask/requirements.txt` file)
- **Yocto Project Environment**: for building custom Linux images
- **Jenkins**: for CI/CD automation
- **Puppet**: for configuration management and deployment

**Note:** For Windows users, consider using **WSL, MSYS2, or Git Bash** with the appropriate packages installed.

---

## Installation and Usage

### Building the C++ Daemon

To compile and build the daemon, ensure that you have the required dependencies installed.
```
make clean
make
```


---

### Running the Flask API

After setting up Python dependencies, the API will be accessible via the specified endpoint.
```
cd ../flask
pip install -r requirements.txt
python app.py
```
The API will be accessible at http://localhost:5000/metrics.

---

### Starting All Services

A shell script is provided to start both the C++ daemon and the Flask API simultaneously, ensuring smooth execution.
```
chmod +x scripts/start_services.sh
./scripts/start_services.sh
```
The daemon will run in the background while the Flask API serves on port 5000.

---

## Yocto Integration

The Yocto recipe builds and installs both the C++ daemon and the Flask application into a custom Linux image. Ensure that your environment is set up correctly before initiating the build process.
```
bitbake system-monitor
```

---

## Jenkins CI/CD Pipeline

The Jenkins pipeline includes the following stages:

1. **Checkout**: Retrieves source code from your Git repository.
2. **Build**: Compiles the C++ daemon.
3. **Testing**: *(Placeholder stage for unit tests.)*
4. **Yocto Build**: Triggers the Yocto build process.
5. **Deploy**: Executes deployment steps, potentially triggering Puppet or Chef.

Set up your Jenkins job to use the `Jenkinsfile` provided in the repository for automated CI/CD.

---

## Puppet Deployment

The Puppet manifest automates the deployment of the `system_monitor` service on target devices. It performs the following tasks:

- Copies the `system_monitor` binary to the appropriate location.
- Configures a **systemd** service for managing the daemon.
- Enables and starts the service automatically.

Applying the manifest ensures seamless service management.

---

## Contributing

Contributions are welcome! If you have improvements, bug fixes, or suggestions, please follow these steps:

1. **Fork** the repository.
2. **Create** a new branch for your feature or bugfix.
3. **Commit** your changes with clear and descriptive messages.
4. **Submit** a pull request detailing your changes.



