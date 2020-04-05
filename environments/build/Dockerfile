# FROM frxyt/xrdp:lxde
FROM frxyt/xrdp@sha256:a6ca8cfbacd3898cd9f2008ad47936a01929b64ba93962092232b31be805f2ec
ENV LANG='en_US.UTF-8' LANGUAGE='en_US:en' LC_ALL='en_US.UTF-8'

RUN echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list \
    && curl https://bazel.build/bazel-release.pub.gpg | apt-key add -

RUN apt-get update && apt-get install -y bazel make g++ pkg-config libx11-dev mesa-common-dev libglu1-mesa-dev libxrandr-dev libxi-dev