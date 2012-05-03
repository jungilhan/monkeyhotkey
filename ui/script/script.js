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

/**
 * 전역 변수
 */
_IsSliding = false;

/**
 * DOM이 로드되었을때 호출되는 핸들러입니다.
 */
$(document).ready(function() {
	initData();
	initUI();
	initEventHandler();	
	
	sendMessage('restore');	
});

/**
 * 데이터를 초기화합니다.
 */
var initData = function() {
	initHotkeyActivityData();
}

/**
 * 핫키 뷰에서 사용하는 데이터를 초기화합니다.
 */
var initHotkeyActivityData = function() {
	hotkeyActivityData = {
		key : '',
		ctrl : false,
		alt : false,
		shift : false
	};		
}

/**
 * UI 초기화합니다.
 */
var initUI = function() {
	initCoActivity();
}

/**
 * 이벤트 핸들러를 초기화합니다.
 */
var initEventHandler = function() {
	bindResizeEvent();
	bindMainViewButtonEvent();
	bindHotkeyViewInputEvent();
	bindHotkeyViewControlBoxEvent();
	bindHotkeyViewConfirmEvent();
	
	bindDeleteViewButtonEvent();	
	bindDeleteViewConfirmEvent();
	
	bindSetupViewOptionEvent();
	bindSetupViewConfirmEvent();
	
	bindInfoViewAnchorEvent();
	bindInfoViewConfirmEvent();
}

/**
 * 메인 뷰의 버튼 이벤트 핸들러를 설정합니다.
 */
var bindMainViewButtonEvent = function() {
	$('#main-activity .listview-item').live('click', function() {
		if ($(this).hasClass('listview-item-guide') == false)
			sendMessage('execute', $(this).index() - 1); // 리스트뷰 헤더를 제외하기 위해 인덱스 1 차감
	});
	
	// 핫키 설정 버튼
	$('#main-activity .listview-item-setup > img').live('click', function() {
		var $owner = $(this).parent().nextAll('.listview-item-content-hotkey');
		var hotkey = $owner.text();

		var $selectedItem = $('.listview-item-content-hotkey.setup-seleted');
		if ($selectedItem.length > 0)
			$selectedItem.removeClass('setup-seleted');
		
		// 클릭 상태 표시
		$owner.addClass('setup-seleted');
				
		// 핫키가 이미 설정되어 있다면 해당 정보를 핫키 액티비티로 전달합니다.
		if (hotkey.length > 0)
			initHotkeyActivityControl(hotkey);

		showActivity('hotkey');		
		return false;
	});
	
	$('#main-activity .listview-item-setup > img').live({
		mouseover: function() {
			$(this).attr('src', 'images/setup18x18_f.png');
		}, 
		mouseout: function() {
			$(this).attr('src', 'images/setup18x18_n.png');
		}
	});
	
	$('#main-activity .listview-item-setup').live('click', function() {
		return false;
	});
	
	$('#main-activity .listview-item').live({
		mouseenter: function() {
			$(this).addClass('listitem-over');
			$removeIcon = $(this).find('.listview-item-remove');
			$removeIcon.show();
		}, 
		mouseleave: function() {
			$(this).removeClass('listitem-over');
			$removeIcon = $(this).find('.listview-item-remove');
			$removeIcon.hide();
		}
	});
	
	$('#main-activity .listview-item-remove').live({
		mouseover: function() {
			$(this).addClass('listview-item-remove-focus');
		}, 
		mouseout: function() {
			$(this).removeClass('listview-item-remove-focus');
		}	
	});
	
	$('#main-activity .listview-item-remove').live('click', function() {	
		// [MEMO] 리스트의 타이틀이 인덱스에 포함되어 있으므로 -1 처리.
		var index = $(this).parents('.listview-item').index() - 1; 
		removeItem(index);

		return false;
	});
}

/**
 * 핫키 설정 텍스트 에디트 이벤트 핸들러를 설정합니다.
 */
var bindHotkeyViewInputEvent = function() {
	$('#hotkey-input').keydown(function(event) {	
		switch (event.keyCode) {
		case 0: case 1: case 2:
		case 3: case 4: case 5:
		case 6: case 7:		
		//case 8: // Backspace
		case 9: // TAB
		case 10: case 11: case 12:		
		case 14: case 15:
		case 16: // Shift
		case 17: // Ctrl
		case 18: // Alt
		case 19:
		case 20: // Caps Lock
		case 21: case 22: case 23:
		case 24: case 25: case 26:	
		case 28: case 29: case 30:
		case 31: case 32: 
		case 33: // Page up
		case 34: // Page down
		case 35: // end
		case 36: // home
		case 37: case 38: case 39: case 40: // 방향키
		case 32: // Space
		case 40: // (
		case 41: // )
		case 42: // *
		case 45: // Insert
		case 64: // @
		case 91: // Left windows
		case 92: // Right windows
		case 94: // ^
		case 106: // Multiply
		case 107: // Add
		case 109: // Substract
		case 110: // Decimal point
		case 111: // divide
		case 144: case 145: 
		case 186: case 187: case 188:
		case 189: case 190: case 191:
		case 192: case 219: case 220:
		case 221: case 222:
		case 229: // 한영
			popup('0~1, A~Z, F1~F12의 문자만 입력할 수 있습니다.', 3000);
			break;
		case 65: case 66: case 67:
		case 68: case 69: case 70:
		case 71: case 72: case 73:
		case 74: case 75: case 76:
		case 77: case 78: case 79:
		case 80: case 81: case 82:
		case 83: case 84: case 85:
		case 86: case 87: case 88:
		case 89: case 90:			
			// 소문자를 대문자로 치환
			$(this).val(String.fromCharCode(event.keyCode).toUpperCase());
			break;
		case 112: $(this).val('F1'); break;
		case 113: $(this).val('F2'); break;
		case 114: $(this).val('F3'); break;
		case 115: $(this).val('F4'); break;
		case 116: $(this).val('F5'); break;
		case 117: $(this).val('F6'); break;			
		case 118: $(this).val('F7'); break;
		case 119: $(this).val('F8'); break;
		case 120: $(this).val('F9'); break;
		case 121: $(this).val('F10'); break;			
		case 122: $(this).val('F11'); break;			
		case 123: $(this).val('F12'); break;		

		case 13: 
			$('#hotkey-ok').click();
			break; // Enter
		
		case 27: // ESC		
		default:
			$(this).val('');
			//alert(event.keyCode);
			return true;
		}
		
		return false;
	});
}

/**
 * 핫키의 제어 버튼(체크박스) 이벤트 핸들러를 설정합니다.
 */
var bindHotkeyViewControlBoxEvent = function() {
    $("#hotkey-activity .cb-enable").click(function() {
        var parent = $(this).parents('.switch');
		
		if ($(this).hasClass('selected')) {
			//alert('[1] checked 2 unchecked');

			$('.cb-enable', parent).removeClass('selected');
			$('.cb-disable', parent).addClass('selected');
			$('.checkbox', parent).attr('checked', false);
			
		} else {
			//alert('[1] unchecked 2 checked');
			
			$('.cb-enable', parent).addClass('selected');
			$('.cb-disable', parent).removeClass('selected');
			$('.checkbox', parent).attr('checked', true);
		}	
    });
	
    $("#hotkey-activity .cb-disable").click(function() {
        var parent = $(this).parents('.switch');
		
		if ($(this).hasClass('selected')) {
			//alert('[2] checked 2 unchecked');

			$('.cb-enable', parent).addClass('selected');
			$('.cb-disable', parent).removeClass('selected');
			$('.checkbox', parent).attr('checked', true);
		} else {
			//alert('[2] unchecked 2 checked');

			$('.cb-enable', parent).removeClass('selected')
			$('.cb-disable', parent).addClass('selected');
			$('.checkbox', parent).attr('checked', false);			
		}
    });
	
	$('#hotkey-activity .switch > label').click(function() {
		//alert($(this).text());
	});
}

/**
 * 핫키의 확인, 취소 버튼의 이벤트 핸들러를 설정합니다.
 */
var bindHotkeyViewConfirmEvent = function() {
	$('#hotkey-ok').click(function() {
		
		// 에디트 창에 입력된 키 저장
		hotkeyActivityData.key = $('#hotkey-input').val();		
		
		// 제어키 상태 저장
		$('#hotkey-activity label.selected').each(function() {
			var control = $(this).text();
			if (control == 'Ctrl')
				hotkeyActivityData.ctrl = true;
			
			if (control == 'Alt')
				hotkeyActivityData.alt = true;
				
			if (control == 'Shift')
				hotkeyActivityData.shift = true;
		});
		
		//alert(hotkeyActivityData.key + ' Ctrl: ' + hotkeyActivityData.ctrl + ' Alt: ' + hotkeyActivityData.alt + ' Shift: ' + hotkeyActivityData.shift);		
		var hotkey = '';
		if (hotkeyActivityData.ctrl)
			hotkey += 'Ctrl + ';
		if (hotkeyActivityData.alt)
			hotkey += 'Alt + ';
		if (hotkeyActivityData.shift)
			hotkey += 'Shift + ';		

		if (hotkeyActivityData.key == '' || hotkey == '') {
			popup('설정이 완료되지 않았습니다. 단축키와 제어키의 조합으로 등록하세요.', 3000);
			return;
		}
		
		if (hotkeyActivityData.key.length > 0)
			hotkey += hotkeyActivityData.key;
		else
			hotkey = hotkey.slice(0, -3);
		
		// 메인 액티비티에 핫키 정보를 출력
		setHotkeyOnMainActivity(hotkey);		
		
		hideActivity('hotkey');
	});
	
	$('#hotkey-cancel').click(function() {
		hideActivity('hotkey');
	});	
}

/**
 * 삭제 액티비티의 확인, 취소 버튼의 이벤트 핸들러를 설정합니다.
 */
var bindDeleteViewConfirmEvent = function() {
	$('#delete-ok').click(function() {		
		var items = new Array();
		
		// 삭제 선택된 아이템의 인덱스를 저장
		$('#delete-activity .delete-selected').each(function() {
			items.push($(this).index());
		});

		// 네이티브로 삭제 메시지 전달 PostMessage
		if (items.length > 0)
			sendMessage('remove', items);
			
		// 메인 액티비티에서 인덱스에 해당되는 아이템을 삭제 (숨김 후 삭제)
		for (var index = 0; index < items.length; index++) {		
			$('#main-activity .listview-item').eq(items[index]).hide();			
		}
		
		$('#main-activity .listview-item:hidden').remove();		
		onCompleteDelete();
		
		hideActivity('delete');
	});
	
	$('#delete-cancel').click(function() {
		hideActivity('delete');
	});	
}

/**
 * 삭제 완료 후 처리해야될 작업을 위한 함수입니다.
 */
var onCompleteDelete = function() {
	// 메인 액티비티의 마지막 아이템 스타일 적용
	if ($('#main-activity .listview-item-last').length == 0) {
		$('#main-activity .listview-item:last').addClass('listview-item-last');
	}
	
	// 메인 액티비티의 모든 아이템이 삭제됐을 경우, 가이드 문장 출력
	if ($('#main-activity .listview-item').length == 0) {
		addGuideItem('images/exclamation.png', '프로그램을 등록하세요', '자주 사용하는 프로그램을 끌어 놓으세요');
	}
}

/**
 * 삭제 액티비티의 삭제할 아이템에 대한 클릭 이벤트 핸들러를 설정합니다.
 */
var bindDeleteViewButtonEvent = function() {
	$('#delete-activity .delete-item').live('click', function() {
		if ($(this).hasClass('delete-selected')) {		
			$(this).removeClass('delete-selected');
			var file = $(this).children('img').attr('src');
			$(this).children('img').eq(0)
									.fadeIn('slow').attr('src', file.replace('.gray', ''));
			
			// 삭제 라벨 숨김
			var $deleteLabel = $(this).children('img').eq(1);
			
			$deleteLabel.animate({
				width: '0px',
				height: '0px'
			}, {
				duration: 'fast', 
				complete: function() {
					$(this).css('visibility', 'hidden');
				}
			});
			
		} else {
			$(this).addClass('delete-selected');
			var file = $(this).children('img').attr('src');
			$(this).children('img').eq(0)
									.fadeIn('slow').attr('src', file + '.gray');												
		
			// 삭제 라벨 표시 
			var $deleteLabel = $(this).children('img').eq(1);
			var labelWidth = '32px';
			var labelHeight = '32px';
			var labelTop = $(this).position().top + 40;
			var labelLeft = $(this).position().left + 60;
			$deleteLabel.css({
						'top': labelTop,
						'left': labelLeft,
						'height': 0,
						'width': 0,
						'visibility': 'visible'
						});
						
			$deleteLabel.animate({
				width: labelWidth,
				height: labelHeight
			}, {
				duration: 'fast'
			});
		}	
	});
	
	$('#delete-activity .delete-item').live({
		mouseover: function() {
			$(this).addClass('deleteitem-over');
		}, 
		mouseout: function() {
			$(this).removeClass('deleteitem-over');
		}
	});	
}

/**
 * 설정 액티비티의 사용자 옵션 설정에 대한 이벤트 핸들러를 설정합니다.
 */
var bindSetupViewOptionEvent = function() {
    $("#setup-activity .cb-enable").click(function() {
        var parent = $(this).parents('.switch');
		
		if ($(this).hasClass('selected')) {
			//alert('[1] checked 2 unchecked');

			$('.cb-enable', parent).removeClass('selected');
			$('.cb-disable', parent).addClass('selected');
			$('.checkbox', parent).attr('checked', false);
			
		} else {
			//alert('[1] unchecked 2 checked');
			
			$('.cb-enable', parent).addClass('selected');
			$('.cb-disable', parent).removeClass('selected');
			$('.checkbox', parent).attr('checked', true);
		}		
    });
	
    $("#setup-activity .cb-disable").click(function() {
        var parent = $(this).parents('.switch');

		if ($(this).hasClass('selected')) {
			//alert('[2] checked 2 unchecked');

			$('.cb-enable', parent).addClass('selected');
			$('.cb-disable', parent).removeClass('selected');
			$('.checkbox', parent).attr('checked', true);
		} else {
			//alert('[2] unchecked 2 checked');

			$('.cb-enable', parent).removeClass('selected')
			$('.cb-disable', parent).addClass('selected');
			$('.checkbox', parent).attr('checked', false);			
		}
    });
	
	// [TODO] 추가되는 옵션이 있을 때 아래에 추가 - 설정 즉시 효과가 나타나야 되는 옵션, 그 외는 확인 버튼 클릭 시 처리
}

/**
 * 설정 액티비티의 확인, 취소 버튼의 이벤트 핸들러를 설정합니다.
 */
var bindSetupViewConfirmEvent = function() {
	$('#setup-ok').click(function() {
		// 옵션 처리 
		var autorun = $('#setup-activity #setup-autorun').is(':checked');
		var topmost = $('#setup-activity #setup-topmost').is(':checked');
		
		// 네이티브로 설정 메시지 전달
		sendMessage('setup', 'autorun', autorun ? 'on' : 'off');
		sendMessage('setup', 'topmost', topmost ? 'on' : 'off');
		
		hideActivity('setup');
	});
	
	$('#setup-cancel').click(function() {
		hideActivity('setup');
	});		
}

/**
 * Anchor에 대한 이벤트 처리를 합니다.
 */
var bindInfoViewAnchorEvent = function() {
	$('#info-activity #update').click(function() {
		sendMessage('update');
		return false;
	});
	
	$('#info-activity .open-url').click(function() {
		sendMessage('openUrl', $(this).attr('href'));
		return false;
	});
}

/**
 * 정보 액티비티의 확인, 취소 버튼의 이벤트 핸들러를 설정합니다.
 */
var bindInfoViewConfirmEvent = function() {
	$('#info-ok').click(function() {	
		hideActivity('info');
	});
}

/**
 * 메인 액티비티 리스트의 hotkey 정보를 설정합니다.
 * @param {hotkey} 핫키 정보
 */
var setHotkeyOnMainActivity = function(hotkey) {
 	var $selectedItem = $('.listview-item-content-hotkey.setup-seleted');
 	
 	if (hotkey.length) {
		var $hotkey = createMark(hotkey);
		$selectedItem.html($hotkey); 
		
 	} else
 		$selectedItem.text('단축키를 설정하세요.');
 		
 	var index = $selectedItem.parents('.listview-item').index();
 	sendMessage('hotkey', index - 1, hotkey);
 }
 
 /**
 * 일반 hotkey 정보를 마크 표시로 시각화 합니다.
 * @param {hotkey} 핫키 정보
 * @return {$Object} jQuery 객체 
 */
var createMark = function(hotkey) {
	var keys = hotkey.split(' + ');
	var $hotkey = $('<span></span>');
	var incomplete = true; // 조합키만 입력된 상태
	
	for (var index = 0; index < keys.length; index++) {
		switch (keys[index]) {
			case 'Ctrl':
				$hotkey.append($('<span class="mark-ctrl">Ctrl</span><span>+</span>'));
				break;
			case 'Alt':
				$hotkey.append($('<span class="mark-alt">Alt</span><span>+</span>'));
				break;
			case 'Shift':
				$hotkey.append($('<span class="mark-shift">Shift</span><span>+</span>'));
				break;
			default:
				incomplete = false;
				$hotkey.append($('<span>' + keys[index] + '</span>+').addClass('mark-key'));
				break;
		}
	}
	
	if (incomplete)
		$hotkey.children().last().remove();

	return $hotkey 
}
 
/**
 * 모든 코-액티비티를 초기화합니다. 
 */
var initCoActivity = function() {
	initHotkeyActivity();
	initDeleteActivity();
	initSetupActivity();
	initInfoActivity();
}

/**
 * 특정 액티비티를 제외한 모든 코-액티비티를 초기화합니다.
 * @param {$activity} 초기화하지 않을 액티비티
 */
var initActivityOthers = function($activity) {
	sendMessage('scrollTop');
	$('#main-activity').css('opacity', '0');

	switch($activity.get(0)) {
	case $('#hotkey-activity').get(0):
		initDeleteActivity();
		initSetupActivity();
		initInfoActivity();	
		break;
	case $('#delete-activity').get(0):
		initHotkeyActivity();
		initSetupActivity();
		initInfoActivity();
		break;
	case $('#setup-activity').get(0):
		console.log('setup');
		initHotkeyActivity();
		initDeleteActivity();
		initInfoActivity();
		break;
	case $('#info-activity').get(0):
		initHotkeyActivity();
		initDeleteActivity();
		initSetupActivity();
		break;	
	}
}

/**
 * 핫키 설정 액티비티를 초기화합니다. 
 */
var initHotkeyActivity = function() {
	var parentWidth = $(window).width();
	var parentHeight = $(window).height();
	
	// 핫키 설정 액티비티 초기화
	$('#hotkey-activity').css({
		'top': '-' + parentHeight + 'px',
		'width': parentWidth + 'px',
		'height': parentHeight + 'px',
		'display': 'none',
		'z-index': 0
	});	
	
	// 뷰 컨트롤 초기화 - 인풋박스
	$('#hotkey-input').val('');
	
	// 뷰 컨트롤 초기화 - 체크박스
	$('#hotkey-activity label.selected').removeClass('selected');
	$('#hotkey-activity label.cb-disable').addClass('selected');
}

/**
 * 핫키 액티비티의 컨트롤을 매개변수를 기반으로 초기화합니다.
 * @param {hotkey} 이미 핫키가 설정되어 있다면 해당 정보가 전달합니다. 
 */
var initHotkeyActivityControl = function(hotkey) {
	if (hotkey.length == 0)
		return;

	if (!isValidHotkey(hotkey))
		return;
		
	var keys = hotkey.split('+');
	for (var index = 0; index < keys.length; index++) {
		console.log(keys[index]);
		switch(keys[index]) {
		case 'Ctrl':
			if ($("#hotkey-activity .cb-disable").eq(0).hasClass('selected'))
				$('#hotkey-activity .switch > label').eq(0).click();
			break;
		case 'Alt':
			if ($("#hotkey-activity .cb-disable").eq(1).hasClass('selected'))
				$('#hotkey-activity .switch > label').eq(2).click();
			break;
		case 'Shift':
			if ($("#hotkey-activity .cb-disable").eq(2).hasClass('selected'))
				$('#hotkey-activity .switch > label').eq(4).click();
			break;			
		default:
			$('#hotkey-input').val(keys[index]);
			break;
		}
	}
}

/**
 * 핫키 액티비티의 에디트 박스에 표시할 수 없는 않는 문자가 있는지 확인합니다.
 * @param {hotkey} 확인할 문자열을 전달합니다.
 */
var isValidHotkey = function(hotkey) {
	for (var index = 0; index < hotkey.length; index++) {
		if (hotkey.charCodeAt(index) > 255)
			return false;
	}
	
	return true;
}

/**
 * 삭제 액티비티를 초기화합니다. 
 */
var initDeleteActivity = function() {
	var parentWidth = $(window).width();
	var parentHeight = $(window).height();
	
	// 삭제 액티비티 초기화
	$('#delete-activity').css({
		'left': '-' + parentWidth + 'px',
		'top': '0px',
		'width': parentWidth + 'px',
		'height': parentHeight + 'px',
		'display': 'none',
		'z-index': 0
	});
	
	// 뷰 컨트롤 초기화 - 아이콘
	$('#delete-activity .delete-item').each(function() {
		$(this).remove();
	});
}

/**
 * 설정 액티비티를 초기화합니다. 
 */
var initSetupActivity = function() {
	var parentWidth = $(window).width();
	var parentHeight = $(window).height();
	
	// 설정 액티비티 초기화
	$('#setup-activity').css({
		'top': '-' + parentHeight + 'px',
		'width': parentWidth + 'px',
		'height': parentHeight + 'px',
		'display': 'none',
		'z-index': '0'
	});		
}

/**
 * 설정 액티비티의 컨트롤을 초기화합니다.
 * @param {autorun} 자동 실행
 * @param {topmost} 항상 위
 */
var initSetupActivityControl = function(autorun, topmost) {		
	if (autorun == 'on')
		$('#setup-activity .switch > label').eq(0).click();
		
	if (topmost == 'on')
		$('#setup-activity .switch > label').eq(2).click();
}

/**
 * 프로그램 정보 액티비티를 초기화합니다. 
 */
var initInfoActivity = function() {
	var parentWidth = $(window).width();
	var parentHeight = $(window).height();
	
	// 프로그램 정보 액티비티 초기화
	$('#info-activity').css({
		'left': parentWidth + 'px',
		'top': '0px',
		'width': parentWidth + 'px',
		'height': parentHeight + 'px',
		'display': 'none',
		'z-index': 0
	});		
}

/**
 * 루트 윈도우의 리사이즈 핸들러입니다. 
 * 코-액티비티의 사이즈를 업데이트합니다.
 */
var bindResizeEvent = function() {
	$(window).resize(function() {
		resizeCoActivity();
		alignVerticalCenter($('#popup'));
	});
	
	var resizeCoActivity = function() {
		var parentWidth = $(window).width();
		var parentHeight = $(window).height();
		
		$('#hotkey-activity').width(parentWidth).height(parentHeight);
		$('#delete-activity').width(parentWidth).height(parentHeight);
		$('#setup-activity').width(parentWidth).height(parentHeight);
		$('#info-activity').width(parentWidth).height(parentHeight);
	};
}

/**
 * Native 모듈로부터 전달된 메시지를 처리하는 핸들러입니다.
 * @param {message} 메시지 타입
 * @param {arg1} 데이터 1
 * @param {arg2} 데이터 2
 * @param {arg3} 데이터 3
 */
var handleMessage = function(message, arg1, arg2, arg3) {		
	// 디폴트 매개변수 처리
	arg1 = (typeof(arg1) != 'undefined') ? arg1 : '';
	arg2 = (typeof(arg2) != 'undefined') ? arg2 : '';
	arg3 = (typeof(arg3) != 'undefined') ? arg3 : '';	

	switch(message) {
	case 'add':	
		hideAllCoActivity();
		addItem(arg1, arg2);
		break;
	case 'restore':
		restoreItem(arg1, arg2, arg3);
		break;
	case 'setup':
		initSetupActivityControl(arg1, arg2);
		break;
	case 'showDeleteActivity':	
		showActivity('delete');
		break;
	case 'showSetupActivity':
		showActivity('setup');
		break;
	case 'showInfoActivity':
		showActivity('info');
		break;
	case 'goHome':
		hideAllCoActivity();
		break;
	case 'popup':
		popup(arg1, 3000);
		break;
	}
}

/**
 * Native 코드로 메시지를 전달합니다.
 * @param {message} 메시지 타입
 */
var sendMessage = function(message, arg1, arg2) {
	// 디폴트 매개변수 처리
	arg1 = (typeof(arg1) != 'undefined') ? arg1 : '';
	arg2 = (typeof(arg2) != 'undefined') ? arg2 : '';
	
	if (typeof nativeAdapter != 'undefined') {
		nativeAdapter.handleMessage(message, arg1, arg2);	
	} else {
		alert('nativeAdapter is undefined');
	}
};

/**
 * 리스트뷰에 아이템 복원합니다.  
 * @param {icon} 프로그램 아이콘 경로
 * @param {name} 프로그램 이름
 * @param {name} 프로그램 핫키 
 */
var restoreItem = function(icon, name, hotkey) {
	removeGuideItem();
	
	// 아이템 생성
	$listviewItem = createRestoreItem(icon, name, hotkey);
	
	// 리스뷰에 아이템 추가
	$('#listview').append($listviewItem.hide().fadeIn('fast'));	
}

/**
 * 리스트뷰에 아이템을 추가합니다. 
 * @param {icon} 프로그램 아이콘 경로
 * @param {name} 프로그램 이름
 */
var addItem = function(icon, name) {
	removeGuideItem();
	
	// 아이템 생성
	$listviewItem = createItem(icon, name);
	
	// 리스뷰에 아이템 추가
	$('#listview').append($listviewItem.hide().fadeIn('fast'));
};

/**
 * 리스트뷰에 여러 아이템을 추가합니다. 
 * @param {items} 아이템 배열
 */
var addItems = function(items) {
	removeGuideItem();
	
	var animation = function(item) {
		$listviewItem = createItem(item.icon, item.name);
		$('#listview').append($listviewItem.hide().fadeIn('fast', function() {
			if (items.length > 0) {
				animation(items.shift());
			}
		}));
	};
	
	animation(items.shift());
}

/**
 * 리스트뷰에 도움말을 출력하는 아이템을 추가합니다. 
 * 등록한 프로그램이 하나도 없을 때 화면에 보여집니다. 
 * @param {icon} 프로그램 아이콘 경로
 * @param {title} 제목 
 * @param {message} 내용
 */
var addGuideItem = function(icon, title, message) {
	// 아이템 생성
	$listviewItem = createGuideItem(icon, title, message);
	
	// 리스뷰에 아이템 추가
	$('#listview').append($listviewItem.hide().fadeIn('fast'));
};

/**
 * 리스트뷰에 추가할 아이템을 생성합니다.
 * @param {icon} 프로그램 아이콘 경로
 * @param {name} 프로그램 이름
 * @return {$Object} 리스트뷰 아이템
 */
var createItem = function(icon, name) {
	// 현재 리스트뷰의 마지막 아이템 스타일 제거
	$('.listview-item-last').last().removeClass('listview-item-last');
	
	$setup = $('<div></div>').addClass('listview-item-setup').append($('<img />').attr("src",'images/setup18x18_n.png'));
	$remove = $('<img />').addClass('listview-item-remove').attr('src', 'images/remove12x12.png');
	$title = $('<h3>' + name + '</h3>').addClass('listview-item-content-title');
	$hotkey = $('<p>' + '단축키를 설정하세요.' + '</p>').addClass('listview-item-content-hotkey');
	
	// 아이템 생성
	$listviewItem = $('<div></div>').addClass('listview-item').addClass('listview-item-last');
	$listviewitemIcon = $('<img/>').addClass('listview-item-icon').attr("src", icon);
	$listivewIitemContent = $('<div></div>').addClass('listview-item-content')
							.append($setup)
							.append($('<div></div>').append($remove).append($title))
							.append($hotkey);
	
	$listviewItem.append($listviewitemIcon).append($listivewIitemContent);
	
	return $listviewItem;
}

/**
 * 리스트뷰에 추가할 아이템을 복원합니다.
 * @param {icon} 프로그램 아이콘 경로
 * @param {name} 프로그램 이름
 * @param {hotkey} 프로그램 핫키 
 * @return {$Object} 리스트뷰 아이템
 */
var createRestoreItem = function(icon, name, hotkey) {
	// 현재 리스트뷰의 마지막 아이템 스타일 제거
	$('.listview-item-last').last().removeClass('listview-item-last');
	
	var $markHotkey;
	if (hotkey.length > 0) {
		$markHotkey = createMark(hotkey);
	} else {
		$markHotkey = $('<span></span>');
		$markHotkey.text('단축키를 설정하세요.');
	}

	$setup = $('<div></div>').addClass('listview-item-setup').append($('<img />').attr("src",'images/setup18x18_n.png'));
	$remove = $('<img />').addClass('listview-item-remove').attr('src', 'images/remove12x12.png');
	$title = $('<h3>' + name + '</h3>').addClass('listview-item-content-title');
	$hotkey = $('<p>' + $markHotkey.html() + '</p>').addClass('listview-item-content-hotkey');
	
	// 아이템 생성
	$listviewItem = $('<div></div>').addClass('listview-item').addClass('listview-item-last');
	$listviewitemIcon = $('<img/>').addClass('listview-item-icon').attr("src", icon);
	$listivewIitemContent = $('<div></div>').addClass('listview-item-content')
							.append($setup)
							.append($('<div></div>').append($remove).append($title))
							.append($hotkey);
	
	$listviewItem.append($listviewitemIcon).append($listivewIitemContent);
	
	return $listviewItem;
}

/**
 * 리스트뷰에 도움말을 출력하는 아이템을 생성합니다.  
 * @param {icon} 프로그램 아이콘 경로
 * @param {title} 제목
 * @param {message} 내용
 * @return {$Object} 리스트뷰 아이템
 */
var createGuideItem = function(icon, title, message) {
	// 아이템 생성
	$listviewItem = $('<div></div>').addClass('listview-item').addClass('listview-item-last').addClass('listview-item-guide');
	$listviewitemIcon = $('<img/>').addClass('listview-item-icon').attr("src", icon);
	$listivewIitemContent = $('<div></div>').addClass('listview-item-content')
							.append($('<h3>' + title + '</h3>').addClass('listview-item-content-title'))
							.append($('<p>' + message + '</p>').addClass('listview-item-content-hotkey'));	
	
	$listviewItem.append($listviewitemIcon).append($listivewIitemContent);
	
	return $listviewItem;
}
 
/**
 * 리스트뷰에서 아이템을 제거합니다.
 * @param {index} 인덱스
 */
var removeItem = function(index) {
	$('#main-activity .listview-item').eq(index).fadeOut('fast', function(){
		$(this).remove();
		onCompleteDelete();
		sendMessage('remove', index);
	});
}

/**
 * 리스트뷰에서 도움말 아이템을 제거합니다.
 */
var removeGuideItem = function() {	
	$('#main-activity .listview-item-guide').remove();
}

/**
 * 삭제 액티비티의 아이템 리스트를 HTML 로 생성한 뒤 attach 시킨다. 
 @ return {length} 아이템 개수
 */
var requestDeleteItemList = function() {
	$('#main-activity .listview-item').not('.listview-item-guide').each(function() {
		var icon = $(this).children('.listview-item-icon').attr('src');
		var title = $(this).find('.listview-item-content-title').text();
		
		// 아이템 생성						
		var $item = createDeleteItem(title, icon);

		// 액티비티에 아이템 추가
		$('#delete-activity .delete-item-container').append($item);
	});	
	
	return $('#main-activity .listview-item').not('.listview-item-guide').length;
}

/**
 * 삭제 액티비티 표시될 아이템을 생성한다. 
 * @param {title} 프로그램 이름
 * @param {icon} 아이콘
 */
var createDeleteItem = function(title, icon) {
	var $item = $('<div></div>').addClass('delete-item')
								.append($('<img>').attr('src', icon))
								.append($('<img>').addClass('delete-label').attr('src', 'images/delete-label.png').css('visibility', 'hidden'))
								.append($('<p></p>').text(title));
	
	return $item;
}
 
/**
 * 액티비티를 호출합니다.
 * @param {activity} 호출할 액티비티
 */
var showActivity = function(activity) {
	$('.activity').each(function() {
		$(this).css('z-index', '0');
	});
	
	switch (activity) {
	case 'hotkey':
		showHotkeyActivity();
		break;
	case 'delete':		
		showDeleteActivity();
		break;
	case 'setup':
		showSetupActivity();
		break;
	case 'info':
		showInfoActivity();
		break;
	}
}

/**
 * 액티비티를 닫습니다.
 * @param {activity} 종료할 액티비티
 */
var hideActivity = function(activity) {
	$('#main-activity').css('opacity', '1');
	$('#main-activity').attr('tabIndex', -1).focus();	// 포커스 초기화
	
	switch (activity) {
	case 'hotkey':
		hideHotkeyActivity();
		break;
	case 'delete':
		hideDeleteActivity();
		break;
	case 'setup':
		hideSetupActivity();
		break;
	case 'info':
		hideInfoActivity();
		break;
	}
}

/**
 * 모든 코-액티비티를 닫습니다.
 */
var hideAllCoActivity = function() {
	hideActivity('hotkey');
	hideActivity('delete');
	hideActivity('setup');
	hideActivity('info');
}

/**
 * 프로그램 실행을 위한 핫키를 설정하는 액티비티를 호출합니다.
 */
var showHotkeyActivity = function() {
	if (_IsSliding)
		return;
	
	if ($('#hotkey-activity').css('display') != 'none')
		return;
	
	// 뷰 데이터 초기화
	initHotkeyActivityData();
	
	_IsSliding = true;
	var parentHeight = $(window).height();	
	if (hasVerticalScroll())
		parentHeight = $('html').height();
	
	$('#hotkey-activity').css('height', parentHeight + 'px')
						.css('top', '-' + parentHeight + 'px')
						.css('display', 'block')
						.css('z-index', '1')
						.animate({
		top: '0px'
	}, {
		duration: 'slow',
		easing: 'easeInOutQuad',
		complete: function() {			
			_IsSliding = false;
			initActivityOthers($(this));
		}
	});
}

/**
 * 핫키 설정 액티비티를 종료합니다.
 */
var hideHotkeyActivity = function() {	
	if (_IsSliding)
		return;

	if ($('#hotkey-activity').css('display') == 'none')
		return;
	
	_IsSliding = true;
	var parentHeight = $(window).height();
	
	$('#hotkey-activity').animate({
		top: "-=" + parentHeight + 'px'
	}, {
		duration: 'slow',
		easing: 'easeInOutQuad',
		complete: function() {
			_IsSliding = false;
			$(this).css('display', 'none');
			initHotkeyActivity();
		}
	});
}

/**
 * 등록한 프로그램을 삭제하기 위한 액티비티를 호출합니다.
 */
var showDeleteActivity = function() {
	if (_IsSliding)
		return;
	
	if ($('#delete-activity').css('display') != 'none')
		return;
	
	// 삭제 아이템 구성
	if (!requestDeleteItemList()) {
		popup('삭제할 아이템이 없습니다.', 2000);
		return;
	}
	
	_IsSliding = true;
	var parentWidth = $(window).width();	
	var parentHeight = $(window).height();	
	if (hasVerticalScroll())
		parentHeight = $('html').height();

	$('#delete-activity').css('height', parentHeight + 'px')
						.css('left', '-' + parentWidth + 'px')
						.css('display', 'block')
						.css('z-index', '1')
						.animate({
		left: '0px'
	}, {
		duration: 'slow',
		easing: 'easeInOutSine',
		complete: function() {
			_IsSliding = false;
			initActivityOthers($(this));
		}
	});
}

/**
 * 프로그램 삭제 액티비티를 종료합니다.
 */
var hideDeleteActivity = function() {
	if (_IsSliding)
		return;
	
	if ($('#delete-activity').css('display') == 'none')
		return;
	
	_IsSliding = true;
	var parentWidth = $(window).width();
	
	$('#delete-activity').animate({
		left: "-=" + parentWidth + 'px'
	}, {
		duration: 'slow',
		easing: 'easeInOutSine',
		complete: function() {
			_IsSliding = false;
			$(this).css('display', 'none');
			initDeleteActivity();			
		}
	});
}

/**
 * 설정 액티비티를 호출합니다.
 */
var showSetupActivity = function() {
	if (_IsSliding)
		return;
	
	if ($('#setup-activity').css('display') != 'none')
		return;
	
	_IsSliding = true;
	var parentHeight = $(window).height();	
	if (hasVerticalScroll())
		parentHeight = $('html').height();

	$('#setup-activity').css('height', parentHeight + 'px')
						.css('top', '-' + parentHeight + 'px')
						.css('display', 'block')
						.css('z-index', '1')
						.animate({
		top: '0px'
	}, {
		duration: 'slow',
		easing: 'easeInOutQuad',
		complete: function() {
			_IsSliding = false;
			initActivityOthers($(this));
		}
	});
}

/**
 * 설정 액티비티를 종료합니다.
 */
var hideSetupActivity  = function() {
	if (_IsSliding)
		return;

	if ($('#setup-activity').css('display') == 'none')
		return;
	
	_IsSliding = true;
	var parentHeight = $(window).height();
	
	$('#setup-activity').animate({
		top: "-=" + parentHeight + 'px'
	}, {
		duration: 'slow',
		easing: 'easeInOutQuad',
		complete: function() {
			_IsSliding = false;
			$(this).css('display', 'none');
			initSetupActivity();			
		}
	});
}

/**
 * 프로그램 정보를 제공하는 액티비티를 호출합니다.
 */
var showInfoActivity = function() {
	if (_IsSliding)
		return;
	
	if ($('#info-activity').css('display') != 'none')
		return;
	
	_IsSliding = true;
	var parentWidth = $(window).width();
	var parentHeight = $(window).height();	
	if (hasVerticalScroll())
		parentHeight = $('html').height();
	
	$('#info-activity').css('height', parentHeight + 'px')
						.css('left', parentWidth + 'px')
						.css('display', 'block')
						.css('z-index', '1')
						.animate({
		left: '0px'
	}, {
		duration: 'slow',
		easing: 'easeInOutSine',
		complete: function() {
			_IsSliding = false;
			initActivityOthers($(this));			
		}
	});
}

/**
 * 프로그램 정보를 제공하는 액티비티를 종료합니다.
 */
var hideInfoActivity  = function() {
	if (_IsSliding)
		return;
	
	if ($('#info-activity').css('display') == 'none')
		return;
	
	_IsSliding = true;
	var parentWidth = $(window).width();
	
	$('#info-activity').animate({
		left: "+=" + parentWidth + 'px'
	}, {
		duration: 'slow',
		easing: 'easeInOutSine',
		complete: function() {
			_IsSliding = false;
			$(this).css('display', 'none');
			initInfoActivity();
		}
	});
}

/**
 * 수직 스크롤의 여부를 판단합니다.
 */
var hasVerticalScroll = function() {
	var scroll = false;
	
	// html의 height은 실제 컨텐츠의 사이즈를, 
	// window의 height는 현재 화면에 보여지는 영역을 나타내는 것으로 판단됨
	if ($('html').height() > $(window).height())
		return scroll = true;
		
	return scroll;
}

/**
 * 메시지 팝업을 출력합니다.
 */
var popup = function(msg, duration) {
	hidePopup(false);

	var $animateMsg = $('#popup');	
	alignVerticalCenter($animateMsg);
	
	$animateMsg.html(msg);
	$animateMsg.fadeIn(1000);
	
	animateMsgTimer = setTimeout(function() {
		$animateMsg.fadeOut(1000);
	}, duration);
}

/**
 * 메시지 팝업 출력 여부를 반환합니다.
 @ return {shown} 출력 여부
 */
var isPopupShown = function() {
        var shown = false;
        
        if ($('#popup:visible').length > 0)
                return shown;
        
        return shown;   
}

/**
 * 메시지 팝업을 닫습니다.
 */
var hidePopup = function(effect) {
        if ($('#popup:visible').length > 0) {
            window.clearTimeout(animateMsgTimer);
            effect ? $('#popup').fadeOut(1000) : $('#popup').hide();              
        }
}

/**
 * 수평 정렬합니다.
 @ param {$object} 수평 정렬할 jQuery 객체
 */  
var alignVerticalCenter = function($object) {	
	var x = ($(window).width() / 2) - ($object.width() / 2);
	$object.css('left', x - 11);
}
	
/**
 * addItems 테스트 함수
 */
var testAddItems = function() {
	var array = {
		items : [
			{'icon': 'icon0', 'name': 'name0'},
			{'icon': 'icon1', 'name': 'name1'},
			{'icon': 'icon2', 'name': 'name2'},
			{'icon': 'icon3', 'name': 'name3'},
			{'icon': 'icon4', 'name': 'name4'},			
		]
	};
	
	addItems(array.items);
}

/**
 * 핫키 액티비티 호출 테스트 함수
 */
 var testHotkeyActivity = function() {
 	showActivity('hotkey');
	
	setTimeout(function() {
		hideActivity('hotkey');
	}, 3000);
 }
 
 /**
 * 삭제 액티비티 호출 테스트 함수
 */
 var testDeleteActivity = function() {
 	showActivity('delete');
	
	setTimeout(function() {
		hideActivity('delete');
	}, 3000);
 }
 
 /**
 * 설정 액티비티 호출 테스트 함수
 */
 var testSetupActivity = function() {
 	showActivity('setup');
	
	setTimeout(function() {
		hideActivity('setup');
	}, 3000);
 }
 
 /**
 * 정보 액티비티 호출 테스트 함수
 */
 var testInfoActivity = function() {
 	showActivity('info');
	
	setTimeout(function() {
		hideActivity('info');
	}, 3000);
 }
