/*
    Copyright (C) 2012  jungil han <jungil.han@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "0x020000"  // 0xMMNNPP (MM = major, NN = minor, PP = patch)
#define VERSION_STR "2.0.0"

#define DEBUG 0

// [MEMO] Qt Creator 윈도우 버전은 실행 파일을 release, debug 폴더에 생성하기 때문에
//        자동 실행과 같은 방식으로 프로그램 실행시에는 경로가 일치하지 않는 문제 발생.
//        따라서 Windows 바이너리 배포시에는 아래 디파인을 주석처리한다.
//#define QTCREATOR_WIN

#ifdef QTCREATOR_WIN
#define APPLICATION_DIR_PATH QDir::currentPath()
#else
#define APPLICATION_DIR_PATH QCoreApplication::applicationDirPath()
#endif

#define FRAME_WIDTH 332
#define FRAME_HEIGHT 500
#define HEADER_HEIGHT 43
#define FOOTER_HEIGHT 49
#define FOOTER_BUTTON_WIDTH _FRAME_WIDTH / 3;

// [Issue3] 숫자 키패드 처리
#define NUMPAD_START_ID 60000

#endif // CONFIG_H
