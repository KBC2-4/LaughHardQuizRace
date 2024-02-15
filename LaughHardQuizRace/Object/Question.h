#pragma once
#include <string>
#include <vector>
#define MONDAI 40
//
//struct QuizItem {
//	std::string question;			// 質問
//	std::string answer;				// 答え
//	std::vector<std::string> wrongs;// 誤答
//	int difficultyLevel;			// 難易度レベル（1〜5）
//};
//
class QuestionItems
{
private:
    //クラス内で使う変数
    std::vector<std::string> question =
    { "幼稚園、大学生の中で一番大きいのはどれでしょうか？",
        "お酒は20歳から、選挙で投票できるのは18歳からです。\n    では、自動車に乗れるのは何歳からでしょうか？",
        "ボロボロの小屋の扉が風もないのにとじたり、しまったりしています。\n    なぜでしょうか？",
        "学校で非常ベルが鳴りました。みんなが最初にすることはなんでしょうか？",
        "父親と息子が一緒に出掛けましたが、突然雨が降って来たので息子の髪はびしょぬれです。\n    でも、父親の髪は濡れていません。なぜでしょうか？",
        "電話ボックスの中にある鏡は何のためにあるでしょうか？",
        "「白髪を抜くと白髪が増える」という話は有名ですが、実際にはどうでしょうか？",
        "CMを略さずに言うと「コマーシャル」である。マルかバツか？",
        "いまにも泣き出しそうな人がいます。１人はトイレにいて、\n    もう１人はどこにいるでしょう？",
        "名古屋から高速道路に乗り、東京まで時速150kmのスピードで走りました。\n    1時間後にはどの辺りにいるでしょうか？",
        "小屋の扉が風で開いてしまったのを確認しました。しかし、しめることができません。\n    なぜでしょうか？",
        "世界人口ランキング第1位の国は？",
        "学校から10メートル離れた家は、誰の家でしょう？",
        "カエルが10匹並んでいます。その中で一番小さなカエルは大人か子どもかどちらでしょうか？",
        "百から一を引くと何になるでしょうか？",
        "なたは100段以上もある階段から飛び降りました。\n    しかし、一切怪我をしていません。なぜでしょうか？",
        "南極と北極、寒いのはどっちでしょうか？",
        "ブタの形の貯金箱が生れた国はどこでしょうか？",
        "「鰻」これ何と読む？",
        "小便小僧だけではなく、小便少女もいる。マルかバツか？",
        "自由の女神は裸足である？マルかバツか？",
        "パンダのしっぽは黒色。マルかバツか？",
        "友人の家を訪ねたけど留守だったので、くるまで待つことにしました。\n    どこで待っているでしょうか？",
        "400円持って行って200円のお菓子を買いました。おつりはいくらでしょうか？",
        "つよし君とあつし君は血のつながった兄弟で、生年月日も一緒です。\n    しかし双子ではありません。なぜでしょうか？",
        "サザエさんのタラちゃんのお姉ちゃんは誰でしょうか？",
        "足が遅いA君は70キロもあるのに、1分で走ってしまいました。なぜでしょうか？",
        "心臓を3つ持っている生き物は？",
        "喉が痛い時に食べるといいお菓子は？",
        "100を半分で割って10を足しました。いくつになったでしょうか？",
        "中国語はどこの国の言葉でしょうか？",
        "ナイル川と万里の長城どっちが長い？",
        "野球のホームベースは現在はゴム製ですが、昔は違いました。\n    昔は何でできていたでしょうか？",
        "使い捨てのカイロをすぐに温める方法はなんでしょうか？",
        "「鮪」これ何と読む？",
        "かくれんぼで鬼が勝手に帰ると罪に問われるか？マルかバツか？",
        "細菌とウイルス、生物なのは？",
        "日本人口第3位の都道府県はどこ？",
        "「鮎」これ何と読む？",
    };
    std::vector<std::vector<std::string>> answer = {
        { "幼稚園","0歳","何も起こっていない","びっくりした","髪がないから", "後ろに人が並んでいるのを確認するため", "増えることはない", "?",
        "居間", "警察署", "閉まっているから", " インド", "先生", "大人", "しろ","1段目から飛び降りたから","南極","イギリス","うなぎ",
        "〇","?","?","車","０円","三つ子以上だから","いない","短い距離を走ったから","タコ","マシュマロ",
        "12","日本","万里の長城","大理石","息を吹きかける","まぐろ","〇","細菌","大阪府","あゆ"},


        {"大学生" , "18歳", "誰かがいるから", "机の下に隠れた", "帽子を被っていたから", "身だしなみをチェックするため", "本当に増える", "〇",
        "リビング", "東京", "扉が風で飛んでなくなっていたから", "中国", "家族", "子供","99","本当は飛び降りていないから","北極","日本","えつ",
        "?","〇","〇","ドアの前","200円","本当は兄弟じゃないから","ワカメちゃん","車に乗っていたから","くじら","チョコレート",
        "60","中国","ナイル川","貝","揉む","たい","?","ウイルス","神奈川県","さけ"}
    };


    // 難易度
    std::vector<int> difficulty = {
         1 , 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,
        3,3,3,3,3,3,3,3,3,3
    };

public:
    //関数をここで宣言する
    QuestionItems() = default;
    ~QuestionItems() = default;
    // void Update();
    // void Draw();

    std::string GetQuestion(int num) { return question.at(num); }
    std::string GetAnswer(int num, bool correct) { return answer.at(correct).at(num); }




};

