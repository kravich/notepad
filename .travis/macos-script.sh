#!/bin/bash

compile()
{
    brew install qt
    brew install uchardet

    export PATH="/usr/local/opt/qt/bin:$PATH"
    export PKG_CONFIG_PATH="/usr/local/opt/qt/lib/pkgconfig"

    ./configure
    make || return 1
}

deploy()
{
    compile || return 1

    cd out/release
    #macdeployqt notepadng.app  # <-- this would be easy, but it is broken with QtWebEngine

    macdeployqt notepadng.app -executable=notepadng.app/Contents/MacOS/notepadng || return 1
    macdeployqt notepadng.app -executable=notepadng.app/Contents/Frameworks/QtWebEngineCore.framework/Versions/5/Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess || return 1

    MAC_DEPLOY_FIX='../../.travis/tools/macdeployqtfix/macdeployqtfix.py'
    python2.7 $MAC_DEPLOY_FIX notepadng.app/Contents/MacOS/notepadng /usr/local/Cellar/qt/5.* || return 1
    python2.7 $MAC_DEPLOY_FIX notepadng.app/Contents/Frameworks/QtWebEngineCore.framework/Versions/5/Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess /usr/local/Cellar/qt/5.* || return 1

    # Manually create the DMG
    mkdir dmgfolder
    mv notepadng.app dmgfolder
    ln -s /Applications ./dmgfolder/Applications
    hdiutil create -fs HFS+ -format UDBZ -srcfolder ./dmgfolder -volname Notepadng notepadng-${NNG_VERSION}.dmg || return 1
}

if [ "$NNG_BUILD_TYPE" == "DEPLOY" ]; then
    deploy
else
    compile
fi
