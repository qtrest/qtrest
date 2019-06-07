import qbs

StaticLibrary {
    name: "qtrest"
    Depends { name: 'cpp' }
    Depends { name: "Qt.network" }
    Depends { name: "Qt.qml" }
    files: [
        "src/apibase.h",
        "src/models/baserestlistmodel.h",
        "src/models/detailsmodel.h",
        "src/models/restitem.h",
        "src/models/pagination.h",
        "src/models/abstractjsonrestlistmodel.h",
        "src/models/abstractxmlrestlistmodel.h",
        "src/models/jsonrestlistmodel.h",
        "src/models/requests.h",
        "src/models/xmlrestlistmodel.h",
        "src/apibase.cpp",
        "src/models/baserestlistmodel.cpp",
        "src/models/detailsmodel.cpp",
        "src/models/restitem.cpp",
        "src/models/pagination.cpp",
        "src/models/abstractjsonrestlistmodel.cpp",
        "src/models/abstractxmlrestlistmodel.cpp",
        "src/models/jsonrestlistmodel.cpp",
        "src/models/requests.cpp",
        "src/models/xmlrestlistmodel.cpp",
        "com_github_kafeg_qtrest.qrc",
        "README.md",
        "LICENSE",
        "docs/QtMicroRestFramework.qmodel",
    ]
    cpp.includePaths: ["src", "src/models"]
    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["src", "src/models"]
    }
}
