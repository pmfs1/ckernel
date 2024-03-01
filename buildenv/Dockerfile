FROM debian:latest
LABEL org.opencontainers.image.source=https://github.com/root-archive/useful-containers/tree/trunk/buildenv-gcc-cross-x86_64-elf
LABEL org.opencontainers.image.licenses=MIT
LABEL org.opencontainers.image.authors="pmfs1"
ARG BINUTILS_VERSION=2.41
ARG GCC_VERSION=13.2.0
RUN set -x && apt-get update -y && \
	apt-get install -y \
		git \
		curl \
		wget \
		sudo \
		gpg \
		locales \
		build-essential \
		libssl-dev \
		libffi-dev \
		gcc \
		libgmp3-dev \
		libmpfr-dev \
		libisl-dev \
		libmpc-dev \
		texinfo \
		bison \
		flex \
		make \
		bzip2 \
		patch
RUN set -x && \
	mkdir -p /usr/local/src && \
	cd /usr/local/src && \
	wget -q https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.gz && \
	wget -q https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz && \
	tar zxf binutils-${BINUTILS_VERSION}.tar.gz && \
	tar zxf gcc-${GCC_VERSION}.tar.gz && \
	rm binutils-${BINUTILS_VERSION}.tar.gz gcc-${GCC_VERSION}.tar.gz && \
	chown -R root:root binutils-${BINUTILS_VERSION} && \
	chown -R root:root gcc-${GCC_VERSION} && \
	chmod -R o-w,g+w binutils-${BINUTILS_VERSION} && \
	chmod -R o-w,g+w gcc-${GCC_VERSION}
COPY files/src /usr/local/src/
COPY files/gcc/t-x86_64-elf /usr/local/src/gcc-${GCC_VERSION}/gcc/config/i386/
COPY files/gcc/config.gcc.patch /usr/local/src/gcc-${GCC_VERSION}/gcc/
RUN set -x && \
	cd /usr/local/src && \
	bash build-binutils.sh ${BINUTILS_VERSION} && \
	bash build-gcc.sh ${GCC_VERSION}
RUN apt-get update -y && \
	apt-get install -y nasm xorriso grub-pc-bin grub-common qemu-system-x86 && \
	sudo apt-get dist-upgrade -y && \
	sudo apt-get full-upgrade -y && \
	sudo apt-get autoremove -y && \
	sudo apt-get autoclean -y && \
	sudo apt-get clean -y && \
	sudo rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
CMD ["/bin/bash"]
VOLUME /root/env
WORKDIR /root/env
# COPY ./ /root/
# WORKDIR /root/
# ENTRYPOINT build.sh
