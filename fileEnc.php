<?php
/**
 * fileEnc.php
 * 此段程序主要用于接收客户端发送来的用于加密的文件和加密选项，并传回加密后的文件。
 * $inFile为接收的文件
 * $key为接收的密钥
 * $method为加/解密选项，值为"enc"表示加密，值为"dec"表示解密
 *
 * 在取出客户端发送的信息后，将信息组装为系统命令调用可执行文件fileEnc，格式为：./fileEnc 输入文件 输出文件 密钥 加密选项（e或d）
 * 执行完该条命令后，./fileEnc会在web应用目录下生成输出的文件，输出文件名格式为：加密情况下，输出文件名为输入文件名+.en，
 *   解密情况下，输出文件名为输入文件名去掉.en。
 *
 * 程序将输出的文件路径和文件名组装为json格式数据，然后发送回客户端
 *
 */
header("Content-type:text/html;charset=utf-8");
$key = $_POST["key"];
$inFile = $_FILES["inFile"]["tmp_name"];
$method = $_POST["en"];
$outFile = '';
$error = null;

if ($method[0] == 'e')
{
    $outFile = $_FILES["inFile"]["name"].'.enc';
}
else if ($method[0] == 'd')
{
    if (substr($_FILES["inFile"]["name"], -4) != ".enc")
    {
        $error = "Input file is not a .enc file!";
    }
    $outFile = substr($_FILES["inFile"]["name"], 0, -4);
}

if ($error == null)
{
    $command = './fileEnc ' . $inFile . ' ' . $outFile . ' ' . $key . ' ' . $method[0];
    exec($command, $retArr);
}

$json_arr = array("filename" => $outFile, "filesrc" => $outFile, "error"=>$error);
$json_obj = json_encode($json_arr);
echo $json_obj;