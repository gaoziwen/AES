$(document).ready(function() {
    $("#strEncBtn").click(function () {
        var inStr = $("#strEncForm input[name='inStr']").val();
        var key = $("#strEncForm input[name='key']").val();

        //test input pattern
        var patt = new RegExp("^([a-f]|[A-F]|[0-9]){32}$");
        if (!patt.test(inStr)) {
            alert("Input String Wrong!");
            return;
        }

        if (!patt.test(key)) {
            alert("Key Wrong!");
            return;
        }

        //send data and get response
        var content = $("#strEncForm").find("input").serialize();
        $.ajax({
            url:'strEnc.php',
            type:'post',
            dataType:'json',
            data:content,
            success:function(data){
                var res = data.outStr;
                var procedure = data.procedure;
                var keyEx = data.keyEx;

                $("#outStr").val(res);
                $("#procedure").html(procedure);
                $("#keyEx").html(keyEx);
            }
        });
    });

    $("#sboxBtn").click(function () {
        var inStr = $("#sboxForm input[name='inStr']").val();

        //test input pattern
        var patt = new RegExp("^([a-f]|[A-F]|[0-9]){32}$");
        if (!patt.test(inStr)) {
            alert("Input String Wrong!");
            return;
        }

        var content = $("#sboxForm").serialize();
        $.ajax({
            url:'sboxEnc.php',
            type:'post',
            dataType:'json',
            data:content,
            success:function(data){
                var str = data.sboxOut;
                $("#sboxOut").val(str);
            }
        });
    });

    $("#shiftBtn").click(function () {
        var inStr = $("#shiftForm input[name='inStr']").val();

        //test input pattern
        var patt = new RegExp("^([a-f]|[A-F]|[0-9]){32}$");
        if (!patt.test(inStr)) {
            alert("Input String Wrong!");
            return;
        }

        var content = $("#shiftForm").serialize();
        $.ajax({
            url:'shiftEnc.php',
            type:'post',
            dataType:'json',
            data:content,
            success:function(data){
                var str = data.shiftOut;
                $("#shiftOut").val(str);
            }
        });
    });

    $("#mixBtn").click(function () {
        var inStr = $("#mixForm input[name='inStr']").val();

        //test input pattern
        var patt = new RegExp("^([a-f]|[A-F]|[0-9]){32}$");
        if (!patt.test(inStr)) {
            alert("Input String Wrong!");
            return;
        }

        var content = $("#mixForm").serialize();
        $.ajax({
            url:'mixEnc.php',
            type:'post',
            dataType:'json',
            data:content,
            success:function(data){
                var str = data.mixOut;
                $("#mixOut").val(str);
            }
        });
    });

    $("#addBtn").click(function () {
        var inStr = $("#addForm input[name='inStr']").val();
        var key = $("#addForm input[name='key']").val();

        //test input pattern
        var patt = new RegExp("^([a-f]|[A-F]|[0-9]){32}$");
        if (!patt.test(inStr)) {
            alert("Input String Wrong!");
            return;
        }

        if (!patt.test(key)) {
            alert("Key Wrong!");
            return;
        }

        var content = $("#addForm").serialize();
        $.ajax({
            url:'addEnc.php',
            type:'post',
            dataType:'json',
            data:content,
            success:function(data){
                var str = data.addOut;
                $("#addOut").val(str);
            }
        });
    });

    $("#fileBtn").click(function () {
        var key = $("#fileEncForm input[name='key']").val();

        //test input pattern
        var patt = new RegExp("^([a-f]|[A-F]|[0-9]){32}$");
        if (!patt.test(key)) {
            alert("Key Wrong!");
            return;
        }

        var data = new FormData($("#fileEncForm")[0]);
        $.ajax({
            url:'fileEnc.php',
            type:'post',
            dataType:'json',
            data:data,
            cache:false,
            processData:false,
            contentType:false,
            success:function(data){
                //modify
                var filename = data.filename;
                var filesrc = data.filesrc;
                var error = data.error;

                if (error == null) {
                    $("#fileOut").html(filename);
                    $("#fileOut").attr("href", filesrc);
                } else
                {
                    alert(error);
                }
            }
        });
    });
});