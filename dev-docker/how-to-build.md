# How to Build and Use the Apple II+ Emulator Development Container

This document outlines the steps to build the Docker image and use it with VSCode Remote - Containers for developing the `study-6502` Apple II+ emulator.

## Prerequisites

*   Docker installed and running on your system.
*   VSCode installed.
*   VSCode Extension: "Remote - Containers" (by Microsoft) installed.

## 1. Build the Docker Image

First, you need to build the Docker image from the `Dockerfile` located in this directory.

1.  Open your terminal.
2.  Navigate to the `study-6502/dev-docker` directory:
    ```bash
    cd study-6502/dev-docker
    ```
3.  Build the Docker image. We'll tag it as `apple2-emulator-dev`:
    ```bash
    docker build -t apple2-emulator-dev .
    ```
    This command will use the `Dockerfile` in the current directory (`.`) to create an image named `apple2-emulator-dev`.

## 2. Run the Container Manually (Optional, for testing)

While VSCode Remote - Containers handles most of this, you can run the container manually to verify the setup or for command-line access:

```bash
docker run -it --rm -v "$(pwd)/../:/workspaces/study-6502" apple2-emulator-dev bash
```

** RAY NOTE: **
Once the container is running in Mac/Docker, we can simply connect from VSCode.
 - VSCode:"Remote Explorer"


**Explanation of the command:**
*   `-it`: Runs the container in interactive mode with a TTY.
*   `--rm`: Automatically removes the container when it exits.
*   `-v "$(pwd)/../:/workspaces/study-6502"`: This is the crucial volume mapping.
    *   `"$(pwd)/../"`: This resolves to your local `study-6502` project directory (one level up from `dev-docker`).
    *   `:/workspaces/study-6502`: This is the path *inside* the Docker container where your local project will be mounted. This matches the `WORKDIR` specified in the `Dockerfile`.
*   `apple2-emulator-dev`: The name of the Docker image to use.
*   `bash`: The command to run inside the container (starts a bash shell).

## 3. Use with VSCode Remote - Containers

This is the recommended way to develop within the Docker environment.

1.  **Open the `study-6502` project in VSCode:**
    *   Open VSCode.
    *   Go to `File > Open Folder...` and select your main `study-6502` project directory (the parent of `dev-docker`).

2.  **Reopen in Container:**
    *   Once the folder is open, VSCode should detect the `.devcontainer` folder (which can be implicitly created or configured to point to your `dev-docker` setup) and prompt you to "Reopen in Container".
    *   If you don't see the prompt, open the Command Palette (Cmd+Shift+P on macOS, Ctrl+Shift+P on Linux/Windows).
    *   Type and select "Remote-Containers: Open Folder in Container...".
    *   Select your `study-6502` folder. VSCode will then ask you to select a `Dockerfile` or a pre-defined configuration. Choose the `Dockerfile` located in `study-6502/dev-docker/Dockerfile`.

3.  **Development:**
    *   VSCode will now build (if not already built) and start the Docker container, mounting your `study-6502` project into `/workspaces/study-6502` inside the container.
    *   You will have a fully configured development environment with `cmake`, `g++`, and `libsdl2-dev` available.
    *   You can open terminals within VSCode, and they will be running inside the container, allowing you to compile and run your C++ code.

By following these steps, you will have a consistent and isolated development environment for the Apple II+ emulator project.
