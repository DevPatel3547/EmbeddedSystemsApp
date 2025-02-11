SUMMARY = "Embedded System Monitoring Daemon and Flask API"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835f1a8d4556c4b65d18b5f8b5e6f4e"

# Include the source files that you have in your project.
# In this example, we assume you’ve copied system_monitor.cpp and app.py into the recipe's files directory.
SRC_URI = "file://system_monitor.cpp \
           file://app.py"

S = "${WORKDIR}"

# Build the C++ daemon
do_compile() {
    ${CXX} ${CXXFLAGS} ${S}/system_monitor.cpp -o ${S}/system_monitor -pthread
}

do_install() {
    # Install the binary into the standard binary directory
    install -d ${D}${bindir}
    install -m 0755 ${S}/system_monitor ${D}${bindir}/system_monitor

    # Install the Flask app as a configuration file (adjust as needed)
    install -d ${D}${sysconfdir}/system_monitor
    install -m 0644 ${S}/app.py ${D}${sysconfdir}/system_monitor/app.py
}

FILES_${PN} += "${bindir}/system_monitor ${sysconfdir}/system_monitor/app.py"
