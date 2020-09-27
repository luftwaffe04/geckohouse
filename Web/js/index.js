// 읽어온 센서값을 저장할 전역변수
var temperature=0;	var humidity=0;
var water=0;	var food=0;	var feed=0;
var INIT_flag = 0;

$(function() {
	$.Init();
	setInterval(function() { // 주기적으로 php 호출하는 함수
		$.ajax({
			"url" : "./process.php",
			cache : false,
			success : function(data) {
				INIT_flag = 1;
				// php 값을 가져와 가공하여 css 재적용
				$("").text(data);
				var sensor = data.split(',');

				// 읽어온 값을 정수화하여 전역변수에 대입
				temperature = parseInt(sensor[0]);
				humidity = parseInt(sensor[1]);
				water = parseInt((sensor[2]/1024)*100);
				food = parseInt(sensor[3]);
				feed = parseInt(sensor[4]);

				// 읽어온 값에 따라 css 구조 변경
				$.Init();
			}
		});
	}, 2000); // 5초마다

	// #feed 클릭 이벤트 함수
	$("#feed").click(function(event) {
		/* Act on the event */
		if(INIT_flag != 0 && food > 100) {
			// init의 정상적인 실행이 끝난 후
			// food 값이 100보다 작을 때 이벤트가 발생한 경우
			$.ajax({
				"url" : "./feedonclick.php",
				cache : false,
				success : function() {
					INIT_flag = 0;
				}
			});
		}
	});
});

$.Init = function() {
	// css를 변경하는 함수
	$("#tempbar").css("width", temperature+"%");
	$("#humibar").css("width", humidity+"%");
	$("#waterbar").css("width", water+"%");

	$(".temperature .percent").html(temperature+"℃");
	$(".humidity .percent").html(humidity+"%");
	$(".water .percent").html(water+"%");

	if(food > 100) {
		$("#full").html("EMPTY");
		$("#full").css({
			backgroundColor: "#fc6",
			color : "#003"
		});
		$("#feed").css({
			backgroundColor: "#f90",
			color : "#003",
			cursor : "pointer"
		});
	}
	else {
		$("#full").html("FULL");
		$("#full").css({
			backgroundColor: "#f90",
			color : "#fff"
		});
		$("#feed").css({
			backgroundColor: "#fc6",
			color : "#fff",
			cursor : "default"
		});
	}
}
