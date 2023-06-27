#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#define MAXBUFLEN 1024

enum PinyinMode {
    enmPinyinMode_AllUpper = 0,        //全大写
    enmPinyinMode_AllLower,            //全小写
    enmPinyinMode_FirstUpper,        //首字母大写
};

const char* getPinyinByCode(uint32_t code);
void getPinyin(const char* szChinese, char pinyinBuf[],  uint32_t* bufLen, const uint32_t maxBufLen = MAXBUFLEN, const uint32_t mode = enmPinyinMode_AllLower);

int32_t test()
{
    uint32_t bufLen = 0;
    char pinyinBuf[MAXBUFLEN] = { 0 };
    const char* szChinese = "中华人民共和国 People's Republic of China";
    getPinyin(szChinese, pinyinBuf, &bufLen);
    printf("%s %d\n", pinyinBuf, bufLen);
    return 0;
}

void getPinyin(const char* szChinese, char pinyinBuf[],  uint32_t* bufLen, const uint32_t maxBufLen, const uint32_t mode)
{
    *bufLen = 0;
    uint8_t ucHigh, ucLow;
    uint32_t code, i, j;
    const uint32_t chineseLen = strlen(szChinese);
    for (i = 0; i < chineseLen; ++i)
    {
        uint8_t c = szChinese[i];
        // 排除askii 码
        if (isascii(c))
        {
            pinyinBuf[(*bufLen)++] = c;
            continue;
        }
        ucHigh = (uint8_t)szChinese[i];
        ucLow = (uint8_t)szChinese[++i];
        if (ucHigh <= 0xa0 || ucLow <= 0xa0)
        {
            continue;
        }
        else
        {
            code = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;
        }
        const char* pBuf = getPinyinByCode(code);
        for (j = 0; j < strlen(pBuf) && (*bufLen) < maxBufLen; ++j)
        {
            char cc = pBuf[j];
            switch (mode)
            {
            case enmPinyinMode_AllUpper:break;
            case enmPinyinMode_AllLower:cc = tolower(cc); break;
            case enmPinyinMode_FirstUpper:if (j != 0)cc = tolower(cc); break;
            }
            pinyinBuf[(*bufLen)++] = cc;
        }
    }
}
const char* getPinyinByCode(uint32_t code)
{
    switch (code)
    {
    case 6325:
    case 6436:
    case 7571:
    case 7925:
        return "A";
        break;
    case 6263:
    case 6440:
    case 7040:
    case 7208:
    case 7451:
    case 7733:
    case 7945:
    case 8616:
        return "AI";
        break;
    case 5847:
    case 5991:
    case 6278:
    case 6577:
    case 6654:
    case 7281:
    case 7907:
    case 8038:
    case 8786:
        return "AN";
        break;
        return "ANG";
        break;
    case 5974:
    case 6254:
    case 6427:
    case 6514:
    case 6658:
    case 6959:
    case 7033:
    case 7081:
    case 7365:
    case 8190:
    case 8292:
    case 8643:
    case 8701:
    case 8773:
        return "AO";
        break;
    case 6056:
    case 6135:
    case 6517:
    case 7857:
    case 8446:
    case 8649:
    case 8741:
        return "BA";
        break;
    case 6267:
    case 6334:
    case 7494:
        return "BAI";
        break;
    case 5870:
    case 5964:
    case 7851:
    case 8103:
    case 8113:
    case 8418:
        return "BAN";
        break;
    case 6182:
    case 6826:
        return "BANG";
        break;
    case 6165:
    case 7063:
    case 7650:
    case 8017:
    case 8157:
    case 8532:
    case 8621:
        return "BAO";
        break;
    case 5635:
    case 5873:
    case 5893:
    case 5993:
    case 6141:
    case 6703:
    case 7753:
    case 8039:
    case 8156:
    case 8645:
    case 8725:
        return "BEI";
        break;
    case 5946:
    case 5948:
    case 7458:
    case 7928:
        return "BEN";
        break;
    case 6452:
    case 7420:
        return "BENG";
        break;
    case 5616:
    case 5734:
    case 6074:
    case 6109:
    case 6221:
    case 6333:
    case 6357:
    case 6589:
    case 6656:
    case 6725:
    case 6868:
    case 6908:
    case 6986:
    case 6994:
    case 7030:
    case 7052:
    case 7221:
    case 7815:
    case 7873:
    case 7985:
    case 8152:
    case 8357:
    case 8375:
    case 8387:
    case 8416:
    case 8437:
    case 8547:
    case 8734:
        return "BI";
        break;
    case 5650:
    case 5945:
    case 6048:
    case 6677:
    case 6774:
    case 7134:
    case 7614:
    case 7652:
    case 7730:
    case 7760:
    case 8125:
    case 8159:
    case 8289:
    case 8354:
    case 8693:
        return "BIAN";
        break;
    case 7027:
    case 7084:
    case 7609:
    case 7613:
    case 7958:
    case 7980:
    case 8106:
    case 8149:
    case 8707:
    case 8752:
        return "BIAO";
        break;
    case 8531:
        return "BIE";
        break;
    case 5747:
    case 6557:
    case 7145:
    case 7167:
    case 7336:
    case 7375:
    case 7587:
    case 7957:
    case 8738:
    case 8762:
        return "BIN";
        break;
    case 5787:
    case 5891:
    case 6280:
        return "BING";
        break;
    case 5781:
    case 6403:
    case 6636:
    case 7362:
    case 7502:
    case 7771:
    case 7864:
    case 8030:
    case 8404:
    case 8543:
    case 8559:
        return "BO";
        break;
    case 6318:
    case 6945:
    case 7419:
    case 7446:
    case 7848:
    case 7863:
    case 8519:
        return "BU";
        break;
    case 6474:
    case 7769:
        return "CA";
        break;
        return "CAI";
        break;
    case 6978:
    case 7078:
    case 7218:
    case 8451:
    case 8785:
        return "CAN";
        break;
    case 5687:
        return "CANG";
        break;
    case 6448:
    case 6878:
    case 8309:
    case 8429:
        return "CAO";
        break;
    case 6692:
        return "CE";
        break;
    case 6515:
    case 6825:
        return "CEN";
        break;
    case 6465:
        return "CENG";
        break;
    case 6639:
    case 6766:
    case 7017:
    case 7230:
    case 7311:
    case 7322:
    case 7363:
    case 7942:
    case 7979:
    case 8135:
        return "CHA";
        break;
    case 5713:
    case 7846:
    case 8091:
    case 8218:
        return "CHAI";
        break;
    case 5770:
    case 5838:
    case 6159:
    case 6667:
    case 6893:
    case 6904:
    case 6981:
    case 7031:
    case 7086:
    case 7472:
    case 7688:
    case 7966:
    case 8324:
    case 8580:
        return "CHAN";
        break;
    case 5686:
    case 5943:
    case 6041:
    case 6137:
    case 6660:
    case 6568:
    case 6749:
    case 7029:
    case 7047:
    case 7438:
    case 7509:
    case 8680:
        return "CHANG";
        break;
    case 6687:
    case 7443:
    case 8173:
        return "CHAO";
        break;
    case 5969:
    case 7726:
        return "CHE";
        break;
    case 5840:
    case 5863:
    case 6251:
    case 6433:
    case 6923:
    case 7201:
    case 7320:
    case 7755:
    case 8619:
        return "CHEN";
        break;
    case 5609:
    case 5984:
    case 7239:
    case 7263:
    case 7583:
    case 7810:
    case 7881:
    case 7905:
    case 8146:
    case 8241:
    case 8508:
        return "CHENG";
        break;
    case 5749:
    case 6015:
    case 6061:
    case 6319:
    case 6374:
    case 6420:
    case 6445:
    case 6633:
    case 7042:
    case 7523:
    case 7787:
    case 8023:
    case 8101:
    case 8161:
    case 8231:
    case 8304:
    case 8355:
    case 8388:
    case 8489:
    case 8556:
    case 8746:
        return "CHI";
        break;
    case 6091:
    case 6671:
    case 6731:
    case 8409:
    case 8430:
        return "CHONG";
        break;
    case 5717:
    case 6492:
    case 6716:
    case 8112:
    case 8637:
        return "CHOU";
        break;
    case 5601:
    case 5927:
    case 6680:
    case 6732:
    case 7109:
    case 7238:
    case 7290:
    case 7343:
    case 8150:
    case 8260:
    case 8573:
    case 8777:
        return "CHU";
        break;
    case 6285:
    case 6408:
    case 7590:
    case 8563:
        return "CHUAI";
        break;
    case 6622:
    case 6955:
    case 7516:
    case 7843:
    case 8413:
        return "CHUAN";
        break;
    case 6675:
        return "CHUANG";
        break;
    case 5879:
    case 7302:
    case 7319:
        return "CHUI";
        break;
    case 6127:
    case 8040:
    case 8277:
        return "CHUN";
        break;
    case 7401:
    case 8554:
    case 8626:
        return "CHUO";
        break;
        return "CI";
        break;
    case 6075:
    case 6358:
    case 7684:
    case 8043:
    case 8457:
        return "4337 伺";
        break;
    case 6042:
    case 6840:
    case 7085:
    case 7193:
    case 7214:
    case 7240:
        return "CONG";
        break;
    case 7308:
    case 7403:
    case 7577:
        return "COU";
        break;
    case 6180:
    case 6562:
    case 6607:
    case 7367:
    case 8501:
    case 8530:
    case 8577:
        return "CU";
        break;
    case 5764:
    case 6305:
    case 7664:
    case 7973:
        return "CUAN";
        break;
    case 6718:
    case 6145:
    case 6393:
    case 7213:
    case 7333:
    case 7505:
    case 8631:
        return "CUI";
        break;
    case 6666:
    case 8169:
        return "CUN";
        break;
    case 5640:
    case 6547:
    case 7566:
    case 7917:
    case 7983:
    case 8078:
    case 8526:
    case 8567:
        return "CUO";
        break;
    case 6239:
    case 6353:
    case 6410:
    case 6682:
    case 7007:
    case 8155:
    case 8346:
    case 8716:
    case 8718:
        return "DA";
        break;
    case 6004:
    case 6316:
    case 6523:
    case 6942:
    case 7110:
    case 7173:
    case 8776:
        return "DAI";
        break;
    case 5757:
    case 6144:
    case 6402:
    case 7373:
    case 7470:
    case 7781:
    case 8067:
    case 8087:
    case 8185:
    case 8376:
        return "DAN";
        break;
    case 5852:
    case 5942:
    case 6148:
    case 6920:
    case 7724:
    case 7885:
    case 8141:
        return "DANG";
        break;
    case 6322:
    case 6665:
    case 7514:
    case 8478:
        return "DAO";
        break;
    case 7929:
        return "DE";
        break;
    case 6466:
    case 6556:
    case 7413:
    case 7767:
    case 7975:
    case 8403:
        return "DENG";
        break;
    case 5621:
    case 5765:
    case 5814:
    case 5848:
    case 5901:
    case 5970:
    case 6122:
    case 6454:
    case 7023:
    case 7116:
    case 7260:
    case 7306:
    case 7475:
    case 7738:
    case 7758:
    case 7791:
    case 7965:
    case 8438:
    case 8730:
        return "DI";
        break;
    case 6439:
        return "DIA";
        break;
    case 5871:
    case 5967:
    case 6559:
    case 7172:
    case 7868:
    case 8116:
    case 8118:
    case 8401:
    case 8558:
        return "DIAN";
        break;
    case 7886:
    case 8585:
    case 8684:
        return "DIAO";
        break;
    case 5976:
    case 6006:
    case 6273:
    case 6409:
    case 7526:
    case 8012:
    case 8183:
    case 8562:
    case 8688:
        return "DIE";
        break;
    case 5674:
    case 6404:
    case 7164:
    case 7575:
    case 7754:
    case 7814:
    case 8059:
    case 8184:
    case 8490:
        return "DING";
        break;
    case 7891:
        return "DIU";
        break;
    case 5977:
    case 6343:
    case 6520:
    case 6528:
    case 7517:
    case 7543:
    case 7556:
    case 7747:
    case 8020:
        return "DONG";
        break;
    case 6190:
    case 8128:
    case 8229:
    case 8391:
        return "DOU";
        break;
    case 6022:
    case 6429:
    case 6834:
    case 7292:
    case 7525:
    case 8328:
    case 8338:
    case 8739:
    case 8782:
        return "DU";
        break;
    case 7318:
    case 7649:
    case 8393:
        return "DUAN";
        break;
    case 7701:
    case 7713:
    case 7752:
        return "DUI";
        break;
    case 6771:
    case 7632:
    case 7727:
    case 7766:
    case 7779:
    case 7970:
    case 8527:
        return "DUN";
        break;
    case 6345:
    case 6365:
    case 6785:
    case 7122:
    case 7876:
    case 8154:
    case 8566:
        return "DUO";
        break;
    case 5612:
    case 5832:
    case 5844:
    case 5949:
    case 6035:
    case 6113:
    case 6164:
    case 6332:
    case 6721:
    case 6977:
    case 7025:
    case 7378:
    case 7581:
    case 7916:
    case 7941:
    case 8042:
    case 8206:
    case 8689:
        return "E";
        break;
    case 6176:
    case 6284:
        return "EN";
        break;
    case 5706:
    case 6939:
    case 7177:
    case 7879:
    case 8025:
    case 8660:
        return "ER";
        break;
    case 5950:
    case 7732:
        return "FA";
        break;
    case 6212:
    case 6232:
    case 6506:
    case 7283:
    case 7660:
    case 7818:
    case 8576:
        return "FAN";
        break;
    case 5890:
    case 7242:
    case 7853:
    case 8419:
    case 8648:
        return "FANG";
        break;
    case 6032:
    case 6584:
    case 6713:
    case 6839:
    case 6990:
    case 7119:
    case 7328:
    case 7572:
    case 7619:
    case 7673:
    case 7948:
    case 8082:
    case 8267:
    case 8385:
    case 8468:
    case 8613:
    case 8678:
        return "FEI";
        break;
    case 5739:
    case 6915:
    case 7291:
    case 8687:
    case 8787:
        return "FEN";
        break;
    case 5726:
    case 5926:
    case 6155:
    case 6384:
    case 6767:
    case 7731:
        return "FENG";
        break;
        return "FO";
        break;
    case 8330:
        return "FOU";
        break;
    case 5775:
    case 5776:
    case 5914:
    case 6029:
    case 6062:
    case 6119:
    case 6142:
    case 6252:
    case 6327:
    case 6505:
    case 6686:
    case 6870:
    case 6985:
    case 7058:
    case 7066:
    case 7106:
    case 7108:
    case 7285:
    case 7471:
    case 7680:
    case 7741:
    case 7774:
    case 7775:
    case 7823:
    case 7991:
    case 8005:
    case 8222:
    case 8261:
    case 8280:
    case 8283:
    case 8479:
    case 8535:
    case 8538:
    case 8654:
    case 8691:
        return "FU";
        break;
    case 6246:
    case 7056:
    case 7057:
    case 7424:
    case 7837:
        return " GA";
        break;
    case 5604:
    case 5875:
    case 5982:
    case 7414:
    case 7464:
        return "GAI";
        break;
    case 5965:
    case 6053:
    case 6247:
    case 6306:
    case 6779:
    case 6838:
    case 6887:
    case 7104:
    case 7347:
    case 7426:
    case 7723:
    case 8065:
    case 8491:
        return "GAN";
        break;
    case 7716:
    case 7824:
    case 8364:
        return "GANG";
        break;
    case 5626:
    case 5830:
    case 5912:
    case 6227:
    case 7141:
    case 7332:
    case 7334:
    case 7429:
    case 7915:
        return "GAO";
        break;
    case 5610:
    case 5678:
    case 5933:
    case 5957:
    case 6010:
    case 6435:
    case 7092:
    case 7501:
    case 7585:
    case 7749:
    case 7951:
    case 8143:
    case 8220:
    case 8420:
    case 8732:
        return "GE";
        break;
        return "GEI";
        break;
    case 5608:
    case 6102:
    case 6371:
    case 8462:
        return "GEN";
        break;
    case 6376:
    case 6657:
    case 7114:
    case 8665:
        return "GENG";
        break;
    case 7178:
    case 7537:
    case 8228:
    case 8601:
        return "GONG";
        break;
    case 5694:
    case 5824:
    case 6524:
    case 6960:
    case 7037:
    case 7135:
    case 7259:
    case 7477:
    case 7616:
    case 8349:
    case 8384:
    case 8724:
        return "GOU";
        break;
    case 5637:
    case 5812:
    case 6152:
    case 6536:
    case 6773:
    case 7284:
    case 7379:
    case 7484:
    case 7486:
    case 7591:
    case 7617:
    case 7813:
    case 7825:
    case 7860:
    case 7932:
    case 8019:
    case 8083:
    case 8233:
    case 8494:
    case 8593:
    case 8681:
    case 8729:
        return "GU";
        break;
    case 5652:
    case 5820:
    case 6341:
    case 7273:
    case 7550:
    case 8027:
        return "GUA";
        break;
        return "GUAI";
        break;
    case 5736:
    case 6124:
    case 6272:
    case 6842:
    case 7834:
    case 8057:
    case 8170:
    case 8704:
        return "GUAN";
        break;
    case 6359:
    case 6578:
    case 7270:
    case 7555:
        return "GUANG";
        break;
    case 5648:
    case 5659:
    case 6649:
    case 7003:
    case 7277:
    case 7433:
    case 7448:
    case 8007:
    case 8394:
    case 8657:
    case 8712:
        return "GUI";
        break;
    case 5782:
    case 7121:
    case 7762:
    case 8671:
        return "GUN";
        break;
    case 5769:
    case 6266:
    case 6335:
    case 6494:
    case 6538:
    case 6603:
    case 7304:
    case 7529:
    case 8188:
    case 8268:
    case 8269:
        return "GUO";
        break;
    case 7894:
        return "HA";
        break;
    case 6443:
    case 7560:
    case 8516:
        return "HAI";
        break;
    case 5885:
    case 6153:
    case 6294:
    case 6759:
    case 6911:
    case 7447:
    case 7642:
    case 8192:
    case 8205:
    case 8232:
    case 8793:
        return "HAN";
        break;
    case 6776:
    case 7112:
    case 8194:
        return "HANG";
        break;
    case 6179:
    case 6222:
    case 6438:
    case 6467:
    case 6909:
    case 6916:
    case 7427:
    case 8009:
    case 8211:
    case 8226:
        return "HAO";
        break;
    case 5813:
    case 5932:
    case 5954:
    case 6432:
    case 6756:
    case 7434:
    case 7833:
    case 8202:
    case 8234:
    case 8471:
        return "HE";
        break;
        return "HEI";
        break;
        return "HEN";
        break;
    case 6231:
    case 7181:
    case 7276:
        return "HENG";
        break;
    case 5768:
    case 5774:
    case 5807:
    case 6106:
    case 6214:
    case 6216:
    case 6740:
    case 6792:
        return "HONG";
        break;
    case 6009:
    case 6565:
    case 6943:
    case 8090:
    case 8383:
    case 8455:
    case 8655:
    case 8731:
        return "HOU";
        break;
    case 5792:
    case 6392:
    case 6481:
    case 6518:
    case 6609:
    case 6679:
    case 6717:
    case 6816:
    case 6879:
    case 7190:
    case 7346:
    case 7385:
    case 7618:
    case 7635:
    case 7646:
    case 7670:
    case 7672:
    case 7679:
    case 8013:
    case 8032:
    case 8041:
    case 8055:
    case 8343:
    case 8513:
    case 8590:
        return "HU";
        break;
    case 7072:
    case 7275:
    case 7725:
    case 7892:
        return "HUA";
        break;
    case 8555:
        return "HUAI";
        break;
    case 5928:
    case 6140:
    case 6307:
    case 6487:
    case 6621:
    case 6801:
    case 6829:
    case 6881:
    case 6930:
    case 6953:
    case 7157:
    case 7944:
    case 8673:
    case 8763:
        return "HUAN";
        break;
    case 5882:
    case 6569:
    case 6850:
    case 6874:
    case 6956:
    case 7211:
    case 7533:
    case 8105:
    case 8308:
    case 8382:
    case 8692:
        return "HUANG";
        break;
    case 5822:
    case 6078:
    case 6086:
    case 6205:
    case 6352:
    case 6360:
    case 6425:
    case 6736:
    case 6807:
    case 6811:
    case 6971:
    case 7132:
    case 7185:
    case 7445:
    case 7703:
    case 8219:
    case 8319:
    case 8766:
        return "HUI";
        break;
    case 5827:
    case 6638:
    case 6752:
    case 6867:
        return "HUN";
        break;
    case 5669:
    case 6229:
    case 6311:
    case 6475:
    case 6623:
    case 7856:
    case 7933:
    case 7976:
    case 8175:
    case 8322:
        return "HUO";
        break;
    case 5629:
    case 5632:
    case 5662:
    case 5705:
    case 5742:
    case 5952:
    case 6024:
    case 6033:
    case 6193:
    case 6210:
    case 6265:
    case 6320:
    case 6350:
    case 6383:
    case 6507:
    case 6553:
    case 6809:
    case 6976:
    case 7087:
    case 7160:
    case 7165:
    case 7314:
    case 7374:
    case 7410:
    case 7411:
    case 7469:
    case 7473:
    case 7487:
    case 7620:
    case 7722:
    case 7831:
    case 7990:
    case 8002:
    case 8104:
    case 8217:
    case 8337:
    case 8339:
    case 8463:
    case 8550:
    case 8611:
    case 8661:
    case 8674:
    case 8757:
    case 8768:
        return "JI";
        break;
    case 5704:
    case 5903:
    case 6171:
    case 6521:
    case 6804:
    case 6940:
    case 7176:
    case 7409:
    case 7546:
    case 7702:
    case 7882:
    case 7956:
    case 8072:
    case 8142:
    case 8244:
    case 8353:
    case 8434:
    case 8542:
        return "JIA";
        break;
    case 5752:
    case 5841:
    case 5857:
    case 6149:
    case 6183:
    case 6286:
    case 6853:
    case 6931:
    case 6932:
    case 7144:
    case 7237:
    case 7305:
    case 7407:
    case 7415:
    case 7480:
    case 7489:
    case 7506:
    case 7576:
    case 7790:
    case 7921:
    case 8047:
    case 8148:
    case 8340:
    case 8469:
    case 8534:
    case 8561:
    case 8668:
    case 8721:
        return "JIAN";
        break;
    case 6092:
    case 6814:
    case 7113:
    case 7154:
    case 7481:
    case 7768:
    case 8180:
    case 8461:
    case 8488:
        return "JIANG";
        break;
    case 5714:
    case 5753:
    case 6020:
    case 6090:
    case 6256:
    case 6461:
    case 6572:
    case 7015:
    case 7524:
    case 8008:
    case 8052:
    case 8252:
    case 8520:
    case 8551:
    case 8662:
        return "JIAO";
        break;
    case 5806:
    case 5821:
    case 6255:
    case 6414:
    case 7028:
    case 7061:
    case 7278:
    case 7757:
    case 8060:
    case 8201:
    case 8227:
    case 8441:
    case 8658:
    case 8726:
        return "JIE";
        break;
    case 5865:
    case 6103:
    case 6132:
    case 6468:
    case 6643:
    case 6659:
    case 7138:
    case 7210:
    case 7340:
    case 7465:
    case 7478:
    case 8138:
        return "JIN";
        break;
    case 5751:
    case 5869:
    case 6128:
    case 6616:
    case 6729:
    case 6794:
    case 6941:
    case 6982:
    case 7026:
    case 7534:
    case 7554:
    case 7570:
    case 7626:
        return "JIANG";
        break;
    case 6936:
    case 7671:
        return "JIONG";
        break;
    case 5754:
    case 6417:
    case 6746:
    case 7249:
    case 7274:
    case 8015:
    case 8053:
    case 8481:
    case 8761:
        return "JIU";
        break;
    case 5738:
    case 5810:
    case 6036:
    case 6058:
    case 6076:
    case 6268:
    case 6965:
    case 6980:
    case 7202:
    case 7307:
    case 7316:
    case 7323:
    case 7357:
    case 7381:
    case 7488:
    case 7611:
    case 7850:
    case 7924:
    case 8022:
    case 8132:
    case 8153:
    case 8482:
    case 8522:
    case 8565:
    case 8620:
    case 8634:
    case 8722:
        return "JU";
        break;
    case 5918:
    case 6590:
    case 6824:
    case 7280:
    case 7835:
    case 7935:
    case 7952:
    case 8633:
        return "JUAN";
        break;
    case 5642:
    case 5667:
    case 5860:
    case 5939:
    case 6207:
    case 6421:
    case 6457:
    case 6469:
    case 6540:
    case 6617:
    case 7062:
    case 7169:
    case 7286:
    case 7351:
    case 7663:
    case 7967:
    case 8574:
    case 8591:
        return "JUE";
        break;
    case 6260:
    case 8168:
    case 8362:
    case 8769:
        return "JUN";
        break;
    case 5671:
    case 6339:
    case 7544:
        return "KA";
        break;
    case 5660:
    case 5978:
    case 6160:
    case 6673:
    case 6693:
    case 7888:
    case 7920:
    case 7939:
        return "KAI";
        break;
    case 5709:
    case 6108:
    case 7412:
    case 7772:
    case 7811:
        return "KAN";
        break;
    case 5688:
    case 6742:
    case 7854:
        return "KANG";
        break;
    case 6974:
    case 7264:
    case 7491:
    case 7877:
        return "KAO";
        break;
    case 6430:
    case 6519:
    case 6701:
    case 6859:
    case 7076:
    case 7128:
    case 7170:
    case 7380:
    case 7520:
    case 7807:
    case 7861:
    case 7930:
    case 7993:
    case 8066:
    case 8129:
    case 8204:
    case 8282:
    case 8733:
        return "KE";
        break;
    case 8144:
        return "KEN";
        break;
    case 7912:
        return "KENG";
        break;
    case 5737:
    case 6539:
    case 8377:
        return "KONG";
        break;
    case 6050:
    case 6202:
    case 6321:
    case 7778:
    case 8356:
        return "KOU";
        break;
    case 5658:
    case 6005:
    case 6423:
    case 7111:
    case 8728:
        return "KU";
        break;
    case 5708:
        return "KUA";
        break;
    case 5665:
    case 5906:
    case 6364:
    case 6586:
    case 7558:
        return "KUAI";
        break;
    case 8737:
        return "KUAN";
        break;
    case 5818:
    case 5831:
    case 5887:
    case 5959:
    case 6237:
    case 6349:
    case 7094:
    case 7460:
        return "KUANG";
        break;
    case 5624:
    case 5649:
    case 5771:
    case 6162:
    case 6281:
    case 6413:
    case 6416:
    case 6720:
    case 6951:
    case 7450:
    case 7805:
    case 8606:
    case 8743:
        return "KUI";
        break;
    case 6204:
    case 6245:
    case 6458:
    case 6618:
    case 6928:
    case 7152:
    case 7841:
    case 8051:
        return "LIAO";
        break;
    case 5793:
    case 5988:
    case 6270:
    case 6354:
    case 6803:
    case 8483:
    case 8581:
    case 8764:
        return "LIE";
        break;
    case 6194:
    case 6388:
    case 6555:
    case 6662:
    case 6733:
    case 6964:
    case 7361:
    case 7405:
    case 7602:
    case 7812:
    case 8452:
    case 8579:
    case 8775:
        return "LIN";
        break;
    case 5925:
    case 6063:
    case 6342:
    case 6482:
    case 6786:
    case 7117:
    case 7258:
    case 7289:
    case 7418:
    case 8186:
    case 8240:
    case 8465:
    case 8676:
        return "LING";
        break;
    case 6815:
    case 6962:
    case 7082:
    case 7124:
    case 7628:
    case 7654:
    case 7919:
    case 7954:
    case 8050:
    case 8644:
        return "LIU";
        break;
    case 5966:
    case 6055:
    case 6781:
    case 7171:
    case 7248:
    case 7542:
    case 7735:
    case 8110:
        return "LONG";
        break;
    case 5745:
    case 6168:
    case 6422:
    case 6548:
    case 7946:
    case 8092:
    case 8179:
    case 8287:
    case 8735:
        return "LOU";
        break;
    case 6744:
    case 7321:
    case 7586:
    case 7918:
    case 7989:
    case 8158:
        return "Lü";
        break;
    case 5968:
    case 6303:
    case 6464:
    case 6782:
    case 6843:
    case 6885:
    case 6954:
    case 7220:
    case 7251:
    case 7354:
    case 7391:
    case 7404:
    case 7510:
    case 7545:
    case 7969:
    case 8021:
    case 8056:
    case 8392:
    case 8421:
    case 8652:
        return "LU";
        break;
    case 5785:
    case 7014:
    case 7279:
    case 8029:
    case 8639:
        return "LUAN";
        break;
        return "LǖE";
        break;
        return "LUN";
        break;
    case 5732:
    case 5789:
    case 6093:
    case 6259:
    case 6291:
    case 6604:
    case 6788:
    case 6880:
    case 7183:
    case 7301:
    case 7565:
    case 7961:
    case 8107:
    case 8635:
        return "LUO";
        break;
    case 6328:
        return "M";
        break;
    case 6373:
    case 6579:
    case 7054:
    case 7231:
    case 8301:
        return "MA";
        break;
    case 5929:
    case 6104:
    case 8618:
        return "MAI";
        break;
    case 6012:
    case 6503:
    case 7147:
    case 7655:
    case 7960:
    case 8209:
    case 8293:
    case 8709:
    case 8720:
        return "MAN";
        break;
    case 5888:
    case 6861:
    case 7743:
    case 8294:
        return "MANG";
        break;
    case 5783:
    case 6066:
    case 6525:
    case 6787:
    case 7203:
    case 7436:
    case 7483:
    case 7503:
    case 7624:
    case 7714:
    case 7806:
    case 8317:
    case 8754:
        return "MAO";
        break;
    case 6114:
    case 6550:
    case 6613:
    case 6828:
    case 6856:
    case 7325:
    case 7949:
    case 8044:
    case 8139:
    case 8740:
        return "MEI";
        break;
    case 6249:
    case 7643:
    case 7715:
    case 7845:
        return "MEN";
        break;
    case 5934:
    case 6189:
    case 6211:
    case 6734:
    case 7592:
    case 7770:
    case 8221:
    case 8276:
    case 8323:
    case 8427:
    case 8431:
        return "MENG";
        break;
    case 5634:
    case 5855:
    case 6234:
    case 6368:
    case 6455:
    case 6608:
    case 6772:
    case 6921:
    case 6984:
    case 7563:
    case 7682:
    case 8445:
    case 8767:
    case 8771:
        return "MI";
        break;
    case 6770:
    case 6837:
    case 6847:
    case 7579:
    case 7777:
        return "MIAN";
        break;
    case 6387:
    case 6967:
    case 7131:
    case 7149:
    case 7234:
    case 7721:
    case 7780:
    case 8037:
        return "MIAO";
        break;
    case 5631:
    case 6367:
    case 8326:
    case 8390:
        return "MIE";
        break;
    case 6069:
    case 6526:
    case 6741:
    case 6793:
    case 7137:
    case 7168:
    case 7175:
    case 7710:
    case 8710:
    case 8628:
        return "MIN";
        break;
    case 5804:
    case 6088:
    case 6873:
    case 7452:
    case 7808:
    case 8504:
        return "MING";
        break;
        return "MIU";
        break;
    case 5851:
    case 6052:
    case 6175:
    case 6641:
    case 7038:
    case 7366:
    case 7950:
    case 7987:
    case 8102:
    case 8182:
    case 8586:
    case 8588:
    case 8765:
        return "MO";
        break;
    case 5716:
    case 6372:
    case 7788:
    case 8254:
    case 8290:
    case 8642:
        return "MOU";
        break;
    case 5679:
    case 5973:
    case 6057:
    case 6769:
    case 7504:
    case 7866:
        return "MU";
        break;
    case 6437:
        return "N";
        break;
    case 6264:
    case 7539:
    case 7953:
    case 8136:
        return "NA";
        break;
    case 5630:
    case 6021:
    case 6133:
    case 7245:
        return "NAI";
        break;
    case 6411:
    case 6478:
    case 6479:
    case 7310:
    case 7578:
    case 8279:
    case 8486:
        return "NAN";
        break;
    case 6313:
    case 6476:
    case 6646:
    case 7457:
        return "NANG";
        break;
    case 5611:
    case 5981:
    case 6346:
    case 6614:
    case 7207:
    case 7748:
    case 7883:
    case 8245:
        return "NAO";
        break;
    case 5811:
        return "NE";
        break;
        return "NEI";
        break;
    case 7705:
        return "NEN";
        break;
        return "NENG";
        break;
    case 5703:
    case 5972:
    case 6605:
    case 6685:
    case 7439:
    case 7627:
    case 7711:
    case 7794:
    case 7874:
    case 8682:
        return "NI";
        break;
    case 5605:
    case 5994:
    case 7393:
    case 8004:
    case 8651:
    case 8683:
        return "NIAN";
        break;
        return "NIANG";
        break;
    case 6064:
    case 7053:
    case 7569:
    case 8433:
        return "NIAO";
        break;
    case 5877:
    case 6233:
    case 6431:
    case 8208:
    case 8411:
    case 8570:
        return "NIE";
        break;
        return "NIN";
        break;
    case 5690:
    case 6344:
    case 6924:
    case 8187:
        return "NING";
        break;
    case 6580:
    case 6678:
    case 7004:
        return "NIU";
        break;
    case 5715:
    case 6370:
        return "NONG";
        break;
    case 8181:
        return "NOU";
        break;
    case 6983:
    case 7032:
    case 7059:
    case 7069:
        return "NU";
        break;
    case 7704:
    case 7847:
    case 8412:
        return "Nǖ";
        break;
        return "NUAN";
        break;
        return "NUE";
        break;
    case 5748:
    case 6289:
    case 6386:
    case 7927:
        return "NUO";
        break;
    case 6424:
    case 6462:
        return "O";
        break;
    case 5809:
    case 6670:
    case 7417:
    case 8178:
        return "OU";
        break;
    case 6166:
    case 7243:
    case 8365:
        return "PA";
        break;
    case 5729:
    case 6169:
    case 6363:
        return "PAI";
        break;
    case 6761:
    case 6790:
    case 8140:
    case 8165:
    case 8320:
    case 8571:
        return "PAN";
        break;
    case 6561:
    case 6872:
    case 6944:
    case 8306:
        return "PANG";
        break;
    case 6243:
    case 6583:
    case 6650:
    case 7567:
    case 8069:
        return "PAO";
        break;
    case 6446:
    case 6490:
    case 7623:
    case 7934:
    case 8512:
    case 8612:
        return "PEI";
        break;
    case 6852:
        return "PEN";
        break;
    case 6001:
    case 6456:
    case 6681:
    case 8318:
        return "PENG";
        break;
    case 5607:
    case 5682:
    case 5880:
    case 5892:
    case 5915:
    case 5960:
    case 6017:
    case 6037:
    case 6308:
    case 6472:
    case 6647:
    case 6836:
    case 7039:
    case 7102:
    case 7233:
    case 7422:
    case 7802:
    case 7828:
    case 7875:
    case 8117:
    case 8166:
    case 8223:
    case 8271:
    case 8589:
        return "PI";
        break;
    case 5850:
    case 7073:
    case 7490:
    case 7561:
    case 8470:
    case 8568:
        return "PIAN";
        break;
    case 5666:
    case 6449:
    case 7046:
    case 7146:
    case 7372:
    case 7809:
    case 8310:
        return "PIAO";
        break;
    case 6054:
    case 7513:
        return "PIE";
        break;
    case 7041:
    case 6253:
    case 7016:
    case 7315:
    case 7482:
    case 8213:
        return "PIN";
        break;
    case 5723:
    case 7019:
    case 7250:
    case 8650:
        return "PING";
        break;
    case 5647:
    case 5922:
    case 7174:
    case 7839:
    case 7862:
    case 8011:
    case 8345:
        return "PO";
        break;
    case 5786:
    case 6269:
        return "POU";
        break;
    case 5773:
    case 6459:
    case 6863:
    case 6907:
    case 7217:
    case 7511:
    case 7968:
    case 7972:
    case 8575:
        return "PU";
        break;
    case 5633:
    case 5725:
    case 5963:
    case 6027:
    case 6046:
    case 6089:
    case 6129:
    case 6134:
    case 6161:
    case 6213:
    case 6366:
    case 6450:
    case 6508:
    case 6510:
    case 6764:
    case 6831:
    case 7075:
    case 7118:
    case 7187:
    case 7189:
    case 7229:
    case 7271:
    case 7342:
    case 7440:
    case 7605:
    case 7687:
    case 7712:
    case 7751:
    case 8193:
    case 8251:
    case 8264:
    case 8475:
    case 8476:
    case 8572:
    case 8702:
    case 8772:
        return "QI";
        break;
    case 6154:
    case 8736:
        return "QIA";
        break;
    case 5727:
    case 5761:
    case 5868:
    case 6023:
    case 6045:
    case 6071:
    case 6271:
    case 6509:
    case 6705:
    case 6727:
    case 6925:
    case 6926:
    case 6929:
    case 7155:
    case 7293:
    case 7541:
    case 7709:
    case 7852:
    case 8215:
    case 8373:
        return "QIAN";
        break;
    case 6762:
    case 7045:
    case 7341:
    case 7408:
    case 7633:
    case 7926:
    case 7947:
    case 7974:
    case 8163:
    case 8262:
    case 8439:
    case 8536:
        return "QIANG";
        break;
    case 5668:
    case 5829:
    case 5859:
    case 6081:
    case 6529:
    case 6724:
    case 6730:
    case 7352:
    case 7745:
    case 8546:
    case 8719:
        return "QIAO";
        break;
    case 5907:
    case 6711:
    case 7010:
    case 7492:
    case 7938:
    case 8370:
        return "QIE";
        break;
    case 6043:
    case 6276:
    case 6336:
    case 6426:
    case 6463:
    case 6858:
    case 7353:
    case 7923:
    case 8291:
    case 8432:
        return "QIN";
        break;
    case 6060:
    case 6485:
    case 7349:
    case 7764:
    case 8263:
    case 8332:
    case 8368:
    case 8605:
    case 8675:
    case 8784:
        return "QING";
        break;
    case 5886:
    case 6068:
    case 8123:
    case 8243:
    case 8344:
    case 8528:
    case 8638:
        return "QIONG";
        break;
    case 5720:
    case 5947:
    case 6576:
    case 6848:
    case 6947:
    case 6957:
    case 7317:
    case 7468:
    case 8216:
    case 8239:
    case 8288:
    case 8435:
    case 8460:
    case 8690:
    case 8792:
        return "QIU";
        break;
    case 5816:
    case 5930:
    case 6201:
    case 6230:
    case 6511:
    case 6573:
    case 6754:
    case 7219:
    case 7479:
    case 7512:
    case 7552:
    case 7678:
    case 7765:
    case 8119:
    case 8248:
    case 8329:
    case 8480:
    case 8636:
    case 8781:
        return "QU";
        break;
    case 5825:
    case 6085:
    case 6710:
    case 7125:
    case 7390:
    case 7816:
    case 7893:
    case 8273:
    case 8360:
    case 8760:
        return "QUAN";
        break;
    case 6755:
    case 6758:
    case 7708:
        return "QUE";
        break;
    case 6950:
        return "QUN";
        break;
    case 6059:
    case 8237:
    case 8755:
        return "RAN";
        break;
    case 7692:
    case 8006:
        return "RANG";
        break;
    case 6073:
    case 7012:
    case 7267:
        return "RAO";
        break;
        return "RE";
        break;
    case 5680:
    case 6083:
    case 6156:
    case 6631:
    case 7377:
    case 7994:
    case 8137:
        return "REN";
        break;
        return "RENG";
        break;
        return "RI";
        break;
    case 6541:
    case 6585:
    case 7337:
    case 7532:
    case 8278:
        return "RONG";
        break;
    case 8459:
    case 8569:
    case 8723:
        return "ROU";
        break;
    case 6174:
    case 6224:
    case 6473:
    case 6818:
    case 6865:
    case 6906:
    case 7140:
    case 7908:
    case 8164:
    case 8212:
        return "RU";
        break;
    case 7535:
        return "RUAN";
        break;
    case 6039:
    case 6208:
    case 7236:
    case 7803:
    case 8224:
        return "RUI";
        break;
        return "RUN";
        break;
    case 5728:
    case 8372:
        return "RUO";
        break;
    case 5606:
    case 5677:
    case 7493:
    case 7559:
    case 7610:
        return "SA";
        break;
    case 6471:
        return "SAI";
        break;
    case 6644:
    case 7507:
    case 8454:
        return "SAN";
        break;
    case 6290:
    case 7763:
    case 8210:
        return "SANG";
        break;
    case 6003:
    case 7150:
    case 7156:
    case 7593:
    case 8094:
    case 8694:
        return "SAO";
        break;
        return "SE";
        break;
        return "SEN";
        break;
        return "SENG";
        break;
    case 6394:
    case 7606:
    case 7901:
    case 8080:
    case 8436:
    case 8614:
    case 8672:
        return "SHA";
        break;
    case 8507:
        return "SHAI";
        break;
    case 5663:
    case 5808:
    case 5923:
    case 5979:
    case 6047:
    case 6890:
    case 7009:
    case 7051:
    case 7083:
    case 7594:
    case 7844:
    case 8062:
    case 8321:
    case 8414:
    case 8539:
    case 8713:
        return "SHAN";
        break;
    case 5980:
    case 7120:
    case 7368:
    case 7656:
    case 8592:
        return "SHANG";
        break;
    case 5931:
    case 6070:
    case 6891:
    case 7228:
    case 8366:
    case 8425:
        return "SHAO";
        break;
    case 5639:
    case 5760:
    case 6606:
    case 6860:
    case 7608:
    case 7820:
    case 8774:
        return "SHE";
        break;
    case 5837:
    case 6123:
    case 6351:
    case 6841:
    case 7309:
    case 7547:
    case 7982:
    case 8255:
        return "SHEN";
        break;
    case 6551:
    case 7441:
    case 7782:
    case 8347:
        return "SHENG";
        break;
    case 5854:
    case 5985:
    case 6110:
    case 6173:
    case 6317:
    case 7388:
    case 7459:
    case 7634:
    case 7870:
    case 8307:
    case 8334:
    case 8363:
    case 8525:
    case 8669:
    case 8685:
        return "SHI";
        break;
    case 6587:
    case 7123:
    case 8428:
        return "SHOU";
        break;
    case 5731:
    case 5951:
    case 6136:
    case 6283:
    case 6780:
    case 6888:
    case 7013:
    case 7508:
    case 7582:
    case 7988:
        return "SHU";
        break;
    case 6407:
        return "SHUA";
        break;
    case 8316:
        return "SHUAI";
        break;
    case 6737:
    case 6844:
        return "SHUAN";
        break;
    case 7055:
        return "SHUANG";
        break;
        return "SHUI";
        break;
        return "SHUN";
        break;
    case 6184:
    case 6287:
    case 6989:
    case 7335:
    case 7869:
        return "SHUO";
        break;
    case 5643:
    case 5778:
    case 5944:
    case 6348:
    case 6765:
    case 6784:
    case 6889:
    case 7006:
    case 7065:
    case 7133:
    case 7675:
    case 7940:
    case 8024:
    case 8174:
    case 8247:
    case 8351:
        return "SI";
        break;
    case 5801:
    case 6131:
    case 6534:
    case 6552:
    case 6676:
    case 6704:
    case 6833:
    case 8121:
        return "SONG";
        break;
    case 5937:
    case 6220:
    case 6418:
    case 6453:
    case 6640:
    case 6849:
    case 7612:
    case 7804:
    case 7943:
    case 8284:
        return "SOU";
        break;
    case 5777:
    case 5853:
    case 6188:
    case 6428:
    case 6726:
    case 6819:
    case 8389:
    case 8602:
    case 8653:
        return "SU";
        break;
    case 6601:
        return "SUAN";
        break;
    case 5839:
    case 6120:
    case 6901:
    case 6968:
    case 7661:
    case 7785:
    case 7801:
        return "SUI";
        break;
    case 6105:
    case 6588:
    case 6624:
    case 7330:
    case 8632:
        return "SUN";
        break;
    case 6379:
    case 6434:
    case 6442:
    case 7022:
    case 7288:
    case 7792:
    case 8440:
        return "SUO";
        break;
    case 6743:
    case 6866:
    case 6961:
    case 7329:
    case 7719:
    case 7872:
    case 8533:
    case 8703:
        return "TA";
        break;
    case 5902:
    case 6223:
    case 6330:
    case 7070:
    case 7536:
    case 7638:
    case 7849:
    case 8544:
    case 8656:
        return "TAI";
        break;
    case 5916:
    case 6903:
    case 7428:
    case 7694:
    case 7867:
    case 7936:
    case 8191:
        return "TAN";
        break;
    case 5746:
    case 6491:
    case 6871:
    case 7209:
    case 7344:
    case 7906:
    case 7959:
    case 8177:
    case 8305:
    case 8311:
    case 8442:
    case 8517:
        return "TANG";
        break;
    case 5627:
    case 6391:
    case 6812:
    case 7226:
    case 7666:
        return "TAO";
        break;
        return "1845 餐";
        break;
    case 6315:
    case 7693:
    case 7911:
        return "TE";
        break;
    case 7588:
        return "TENG";
        break;
    case 5735:
    case 6709:
    case 6949:
    case 7130:
    case 8035:
    case 8151:
    case 8514:
        return "TI";
        break;
    case 6261:
    case 6735:
    case 6757:
    case 7369:
    case 7817:
        return "TIAN";
        break;
    case 5712:
    case 7686:
    case 8127:
    case 8272:
    case 8352:
    case 8448:
    case 8622:
    case 8670:
    case 8756:
        return "TIAO";
        break;
    case 6138:
    case 8749:
        return "TIE";
        break;
    case 6080:
    case 6167:
    case 7035:
    case 7272:
    case 7890:
    case 8249:
    case 8610:
        return "TING";
        break;
    case 5701:
    case 5758:
    case 6077:
    case 6444:
    case 6690:
    case 6892:
    case 7737:
        return "TONG";
        break;
    case 7855:
    case 7822:
    case 8727:
        return "TOU";
        break;
    case 6002:
    case 6117:
    case 6143:
    case 7842:
    case 8509:
        return "TU";
        break;
    case 6250:
    case 6972:
        return "TUAN";
        break;
    case 7653:
        return "TUI";
        break;
    case 5759:
    case 6629:
    case 7453:
    case 7564:
        return "TUN";
        break;
    case 5617:
    case 5702:
    case 5971:
    case 6653:
    case 6791:
    case 7256:
    case 7262:
    case 7350:
    case 7740:
    case 8374:
    case 8502:
    case 8541:
    case 8630:
        return "TUO";
        break;
    case 5684:
    case 7020:
    case 7580:
        return "WA";
        break;
        return "WAI";
        break;
    case 5664:
    case 6025:
    case 6150:
    case 7093:
    case 7126:
    case 7194:
    case 7568:
    case 7821:
    case 8274:
        return "WAN";
        break;
    case 5672:
    case 6244:
    case 6715:
    case 7394:
    case 8745:
        return "WANG";
        break;
    case 5743:
    case 5835:
    case 5881:
    case 5883:
    case 6158:
    case 6217:
    case 6488:
    case 6501:
    case 6543:
    case 6545:
    case 6611:
    case 6612:
    case 6739:
    case 6777:
    case 6802:
    case 6822:
    case 6952:
    case 7024:
    case 7166:
    case 7224:
    case 7406:
    case 7631:
    case 7648:
    case 8084:
    case 8426:
    case 8659:
        return "WEI";
        break;
    case 5656:
    case 6751:
    case 6775:
    case 7223:
    case 8609:
        return "WEN";
        break;
    case 6178:
    case 6219:
        return "WENG";
        break;
    case 5733:
    case 6111:
    case 6502:
    case 6855:
    case 7531:
    case 7750:
    case 8627:
        return "WO";
        break;
    case 5603:
    case 5685:
    case 5867:
    case 5889:
    case 5956:
    case 6044:
    case 6377:
    case 6648:
    case 6668:
    case 6672:
    case 6820:
    case 6927:
    case 6935:
    case 6992:
    case 7036:
    case 7080:
    case 7227:
    case 7485:
    case 7641:
    case 8036:
    case 8045:
    case 8077:
    case 8258:
    case 8640:
    case 8789:
        return "WU";
        break;
    case 5750:
    case 5766:
    case 5884:
    case 5913:
    case 6130:
    case 6163:
    case 6191:
    case 6241:
    case 6381:
    case 6567:
    case 6630:
    case 6750:
    case 6827:
    case 6832:
    case 6979:
    case 7050:
    case 7184:
    case 7356:
    case 7456:
    case 7474:
    case 7604:
    case 7668:
    case 7689:
    case 7691:
    case 8010:
    case 8122:
    case 8265:
    case 8303:
    case 8312:
    case 8410:
    case 8424:
    case 8443:
    case 8449:
    case 8466:
    case 8521:
    case 8791:
        return "XI";
        break;
    case 6340:
    case 6582:
    case 6958:
    case 7206:
    case 7252:
    case 7744:
    case 8093:
    case 8333:
    case 8779:
        return "XIA";
        break;
    case 5794:
    case 5823:
    case 6040:
    case 6118:
    case 6226:
    case 6513:
    case 6593:
    case 6963:
    case 7021:
    case 7515:
    case 7662:
    case 7676:
    case 8034:
    case 8079:
    case 8225:
    case 8358:
    case 8444:
    case 8503:
    case 8548:
    case 8549:
    case 8617:
        return "XIAN";
        break;
    case 6028:
    case 6157:
    case 6635:
    case 6652:
    case 7088:
    case 7129:
    case 8313:
    case 8663:
    case 8747:
        return "XIANG";
        break;
    case 6356:
    case 6537:
    case 6876:
    case 6948:
    case 7071:
    case 7115:
    case 7241:
    case 7253:
    case 8257:
    case 8367:
    case 8379:
    case 8744:
        return "XIAO";
        break;
    case 5741:
    case 5784:
    case 5936:
    case 5938:
    case 6215:
    case 6302:
    case 6619:
    case 6661:
    case 6845:
    case 6912:
    case 6966:
    case 7105:
    case 7151:
    case 7331:
    case 7339:
    case 8583:
        return "XIE";
        break;
    case 5622:
    case 6016:
    case 7431:
    case 7607:
    case 8646:
        return "XIN";
        break;
    case 5874:
    case 6084:
    case 6309:
    case 6712:
    case 7742:
        return "XING";
        break;
    case 6026:
        return "XIONG";
        break;
    case 6361:
    case 6522:
    case 6642:
    case 6651:
    case 6869:
    case 8028:
    case 8587:
    case 8759:
        return "XIU";
        break;
    case 5828:
    case 5935:
    case 5955:
    case 6203:
    case 6810:
    case 6851:
    case 7179:
    case 7282:
    case 7667:
    case 7776:
    case 8167:
    case 8458:
    case 8515:
        return "XU";
        break;
    case 5756:
    case 5846:
    case 6170:
    case 6279:
    case 6789:
    case 6854:
    case 6886:
    case 7215:
    case 7324:
    case 7449:
    case 7637:
    case 7651:
    case 7759:
    case 7871:
    case 7964:
    case 8071:
        return "XUAN";
        break;
    case 5842:
    case 7720:
    case 8529:
    case 8708:
        return "XUE";
        break;
    case 5767:
    case 5908:
    case 5987:
    case 6087:
    case 6101:
    case 6206:
    case 6225:
    case 6530:
    case 6563:
    case 6620:
    case 6694:
    case 6813:
    case 6817:
    case 7454:
    case 8131:
    case 8524:
    case 8664:
        return "XUN";
        break;
    case 5683:
    case 5975:
    case 6275:
    case 6512:
    case 6934:
    case 7011:
    case 7180:
    case 7266:
    case 7518:
    case 7728:
    case 7793:
    case 8073:
        return "YA";
        break;
    case 5641:
    case 5645:
    case 5718:
    case 5740:
    case 5780:
    case 5861:
    case 5917:
    case 5919:
    case 6030:
    case 6146:
    case 6535:
    case 6691:
    case 6738:
    case 6753:
    case 6846:
    case 6857:
    case 6991:
    case 7044:
    case 7192:
    case 7360:
    case 7444:
    case 7557:
    case 7645:
    case 7827:
    case 8359:
    case 8506:
    case 8742:
    case 8748:
    case 8790:
        return "YAN";
        break;
    case 6564:
    case 6683:
    case 7630:
    case 7640:
    case 7706:
    case 8253:
    case 8717:
        return "YANG";
        break;
    case 5618:
    case 5619:
    case 6326:
    case 6542:
    case 6570:
    case 7159:
    case 7182:
    case 7235:
    case 7387:
    case 7455:
    case 7540:
    case 7902:
    case 8046:
    case 8126:
    case 8477:
    case 8705:
        return "YAO";
        break;
    case 5644:
    case 5843:
    case 5894:
    case 6262:
    case 7442:
    case 7639:
    case 7884:
        return "YE";
        break;
    case 5655:
    case 5657:
    case 5670:
    case 5693:
    case 5711:
    case 5817:
    case 5961:
    case 5992:
    case 6018:
    case 6051:
    case 6072:
    case 6218:
    case 6236:
    case 6240:
    case 6258:
    case 6314:
    case 6329:
    case 6355:
    case 6362:
    case 6441:
    case 6470:
    case 6527:
    case 6558:
    case 6602:
    case 6634:
    case 6688:
    case 6689:
    case 6708:
    case 6884:
    case 6938:
    case 7068:
    case 7143:
    case 7376:
    case 7383:
    case 7461:
    case 7629:
    case 7658:
    case 7784:
    case 7838:
    case 7955:
    case 7978:
    case 8074:
    case 8089:
    case 8115:
    case 8120:
    case 8270:
    case 8415:
    case 8464:
    case 8472:
    case 8493:
    case 8780:
        return "YI";
        break;
    case 5623:
    case 5920:
    case 5983:
    case 6007:
    case 6065:
    case 6337:
    case 6419:
    case 6594:
    case 6625:
    case 6806:
    case 7519:
    case 7887:
    case 8111:
    case 8230:
    case 8615:
    case 8624:
        return "YIN";
        break;
    case 5788:
    case 5911:
    case 6067:
    case 6094:
    case 6126:
    case 6151:
    case 6186:
    case 6292:
    case 6451:
    case 6663:
    case 6862:
    case 6875:
    case 6913:
    case 7188:
    case 7212:
    case 7326:
    case 7584:
    case 8048:
    case 8108:
    case 8203:
    case 8331:
        return "YING";
        break;
    case 6401:
        return "YO";
        break;
    case 5724:
    case 5953:
    case 6013:
    case 6415:
    case 6728:
    case 7163:
    case 7962:
    case 8014:
    case 8711:
    case 8751:
        return "YONG";
        break;
    case 5653:
    case 5692:
    case 5707:
    case 6112:
    case 6115:
    case 6121:
    case 6347:
    case 6483:
    case 6922:
    case 7254:
    case 7364:
    case 7527:
    case 7880:
    case 8064:
    case 8236:
    case 8242:
    case 8286:
    case 8647:
    case 8778:
    case 8788:
        return "YOU";
        break;
    case 5614:
    case 5625:
    case 5681:
    case 5722:
    case 5836:
    case 5845:
    case 6139:
    case 6187:
    case 6277:
    case 6484:
    case 6486:
    case 6546:
    case 6592:
    case 6632:
    case 6637:
    case 6655:
    case 6748:
    case 6987:
    case 6993:
    case 7005:
    case 7090:
    case 7204:
    case 7437:
    case 7476:
    case 7573:
    case 7603:
    case 7622:
    case 7647:
    case 7659:
    case 7718:
    case 7858:
    case 8033:
    case 8054:
    case 8085:
    case 8086:
    case 8130:
    case 8133:
    case 8266:
    case 8285:
    case 8336:
    case 8407:
    case 8408:
    case 8607:
    case 8625:
        return "YU";
        break;
    case 5989:
    case 6011:
    case 6282:
    case 6768:
    case 7034:
    case 7205:
    case 7358:
    case 7528:
    case 7783:
    case 8016:
    case 8302:
    case 8378:
    case 8629:
        return "YUAN";
        break;
    case 5763:
    case 6914:
    case 7348:
    case 7530:
    case 7865:
        return "YUE";
        break;
    case 5909:
    case 6031:
    case 6581:
    case 6702:
    case 6719:
    case 7101:
    case 7225:
    case 7370:
    case 7432:
    case 7521:
    case 7657:
        return "YUN";
        break;
    case 6257:
    case 6338:
        return "ZA";
        break;
    case 6544:
    case 7162:
        return "ZAI";
        break;
    case 7222:
    case 7435:
    case 8402:
    case 8456:
    case 8485:
    case 8641:
        return "ZAN";
        break;
    case 6242:
    case 7064:
    case 7416:
        return "ZANG";
        break;
    case 6380:
        return "ZAO";
        break;
    case 5638:
    case 8369:
    case 5651:
    case 6385:
    case 6493:
    case 6937:
    case 7430:
    case 8348:
    case 8423:
        return "ZE";
        break;
        return "ZEI";
        break;
    case 5858:
        return "ZEN";
        break;
    case 7153:
    case 7421:
    case 7832:
    case 7913:
        return "ZENG";
        break;
    case 6610:
    case 6274:
    case 6324:
    case 6369:
    case 6378:
    case 7736:
    case 8068:
    case 8238:
    case 8794:
        return "ZHA";
        break;
    case 7746:
    case 8109:
        return "ZHAI";
        break;
    case 5862:
    case 6288:
    case 7625:
        return "ZHAN";
        break;
    case 5675:
    case 5921:
    case 6504:
    case 6554:
    case 6615:
    case 7049:
    case 7216:
    case 8315:
        return "ZHANG";
        break;
    case 5815:
    case 7294:
    case 7840:
    case 8341:
        return "ZHAO";
        break;
    case 5856:
    case 6301:
    case 7247:
    case 7392:
    case 7761:
    case 8049:
    case 8162:
    case 8256:
    case 8487:
        return "ZHE";
        break;
    case 5958:
    case 6172:
    case 6805:
    case 7139:
    case 7269:
    case 7327:
    case 7384:
    case 7466:
    case 7551:
    case 7562:
    case 7685:
    case 7819:
    case 8001:
    case 8018:
    case 8380:
        return "ZHEN";
        break;
    case 5826:
    case 6531:
    case 6571:
    case 7859:
    case 7903:
    case 8361:
        return "ZHENG";
        break;
    case 5620:
    case 5876:
    case 5904:
    case 5990:
    case 6038:
    case 6293:
    case 6489:
    case 6669:
    case 6973:
    case 6975:
    case 7079:
    case 7246:
    case 7255:
    case 7257:
    case 7268:
    case 7382:
    case 7389:
    case 7462:
    case 7553:
    case 7589:
    case 7677:
    case 7683:
    case 7773:
    case 7984:
    case 8026:
    case 8075:
    case 8246:
    case 8474:
    case 8505:
    case 8537:
    case 8557:
    case 8560:
    case 8584:
    case 8603:
        return "ZHI";
        break;
    case 5803:
    case 7981:
    case 8314:
    case 8417:
    case 8564:
        return "ZHONG";
        break;
    case 6107:
    case 6390:
    case 7008:
    case 7091:
    case 7107:
    case 7548:
    case 7756:
    case 8406:
    case 8492:
        return "ZHOU";
        break;
    case 5689:
    case 5710:
    case 5905:
    case 6049:
    case 6079:
    case 6808:
    case 6830:
    case 6883:
    case 7244:
    case 7338:
    case 7345:
    case 7636:
    case 7889:
    case 8070:
    case 8081:
    case 8335:
    case 8371:
    case 8422:
    case 8467:
    case 8578:
    case 8770:
        return "ZHU";
        break;
        return "ZHUA";
        break;
        return "ZHUAI";
        break;
    case 6389:
    case 6645:
    case 8207:
        return "ZHUAN";
        break;
    case 5755:
        return "ZHUANG";
        break;
    case 6723:
    case 7077:
    case 7136:
        return "ZHUI";
        break;
    case 7538:
    case 8124:
        return "ZHUN";
        break;
    case 5730:
    case 5834:
    case 6310:
    case 6823:
    case 6835:
    case 6910:
    case 7644:
    case 7690:
    case 7729:
    case 7977:
        return "ZHUO";
        break;
    case 5849:
    case 6549:
    case 7002:
    case 7060:
    case 7127:
    case 7287:
    case 7402:
    case 7463:
    case 7707:
    case 7786:
    case 7937:
    case 7986:
    case 8172:
    case 8342:
    case 8450:
    case 8484:
    case 8594:
    case 8604:
    case 8623:
    case 8686:
    case 8758:
        return "ZI";
        break;
    case 5744:
    case 7574:
    case 8453:
        return "ZONG";
        break;
    case 5833:
    case 5878:
    case 5924:
    case 7067:
    case 8677:
        return "ZOU";
        break;
    case 5762:
    case 6147:
    case 7963:
        return "ZU";
        break;
    case 6312:
    case 7158:
    case 8582:
        return "ZUAN";
        break;
    case 6209:
        return "ZUI";
        break;
    case 6304:
    case 7355:
    case 8714:
        return "ZUN";
        break;
    case 5872:
    case 6382:
    case 6460:
    case 6684:
    case 7549:
    case 7681:
        return "ZUO";
        break;
    default:
        if (code >= 1601 && code <= 1602)
        {
            return "A";
            break;
        }
        if (code >= 1603 && code <= 1615)
        {
            return "AI";
            break;
        }
        if (code >= 1616 && code <= 1624)
        {
            return "AN";
            break;
        }
        if (code >= 1625 && code <= 1627)
        {
            return "ANG";
            break;
        }
        if (code >= 1628 && code <= 1636)
        {
            return "AO";
            break;
        }
        if (code >= 1637 && code <= 1654)
        {
            return "BA";
            break;
        }
        if (code >= 1655 && code <= 1662)
        {
            return "BAI";
            break;
        }
        if (code >= 1663 && code <= 1677)
        {
            return "BAN";
            break;
        }
        if (code >= 1678 && code <= 1689)
        {
            return "BANG";
            break;
        }
        if (code >= 1690 && code <= 1712)
        {
            return "BAO";
            break;
        }
        if (code >= 1713 && code <= 1727)
        {
            return "BEI";
            break;
        }
        if (code >= 1728 && code <= 1731)
        {
            return "BEN";
            break;
        }
        if (code >= 1732 && code <= 1737)
        {
            return "BENG";
            break;
        }
        if (code > 1738 && code <= 1761)
        {
            return "BI";
            break;
        }
        if (code >= 1762 && code <= 1773)
        {
            return "BIAN";
            break;
        }
        if (code >= 1774 && code <= 1777)
        {
            return "BIAO";
            break;
        }
        if (code >= 1778 && code <= 1781)
        {
            return "BIE";
            break;
        }
        if (code >= 1782 && code <= 1787)
        {
            return "BIN";
            break;
        }
        if (code >= 1788 && code <= 1794)
        {
            return "BING";
            break;
        }
        if (code >= 1801 && code <= 1802)
        {
            return "BING";
            break;
        }
        if (code >= 1803 && code <= 1821)
        {
            return "BO";
            break;
        }
        if (code >= 1822 && code <= 1832)
        {
            return "BU";
            break;
        }
        if (code == 1833)
        {
            return "CA";
            break;
        }
        if (code >= 1834 && code <= 1844)
        {
            return "CAI";
            break;
        }
        if (code >= 1845 && code <= 1851)
        {
            return "CAN";
            break;
        }
        if (code >= 1852 && code <= 1856)
        {
            return "CANG";
            break;
        }
        if (code >= 1857 && code <= 1861)
        {
            return "CAO";
            break;
        }
        if (code >= 1862 && code <= 1866)
        {
            return "CE";
            break;
        }
        if (code >= 1867 && code <= 1868)
        {
            return "CENG";
            break;
        }
        if (code >= 1869 && code <= 1879)
        {
            return "CHA";
            break;
        }
        if (code >= 1880 && code <= 1882)
        {
            return "CHAI";
            break;
        }
        if (code >= 1883 && code <= 1892)
        {
            return "CHAN";
            break;
        }
        if (code >= 1893 && code <= 1911)
        {
            return "CHANG";
            break;
        }
        if (code >= 1912 && code <= 1920)
        {
            return "CHAO";
            break;
        }
        if (code >= 1921 && code <= 1926)
        {
            return "CHE";
            break;
        }
        if (code >= 1927 && code <= 1936)
        {
            return "CHEN";
            break;
        }
        if (code >= 1937 && code <= 1951)
        {
            return "CHENG";
            break;
        }
        if (code >= 1952 && code <= 1967)
        {
            return "CHI";
            break;
        }
        if (code >= 1968 && code <= 1972)
        {
            return "CHONG";
            break;
        }
        if (code >= 1973 && code <= 1984)
        {
            return "CHOU";
            break;
        }
        if (code >= 1985 && code <= 2006)
        {
            return "CHU";
            break;
        }
        if (code == 2007)
        {
            return "CHUAI";
            break;
        }
        if (code >= 2008 && code <= 2014)
        {
            return "CHUAN";
            break;
        }
        if (code >= 2015 && code <= 2020)
        {
            return "CHUANG";
            break;
        }
        if (code >= 2021 && code <= 2025)
        {
            return "CHUI";
            break;
        }
        if (code >= 2026 && code <= 2032)
        {
            return "CHUN";
            break;
        }
        if (code >= 2033 && code <= 2034)
        {
            return "CHUO";
            break;
        }
        if (code >= 2035 && code <= 2046)
        {
            return "CI";
            break;
        }
        if (code >= 2047 && code <= 2052)
        {
            return "CONG";
            break;
        }
        if (code >= 2054 && code <= 2057)
        {
            return "CU";
            break;
        }
        if (code >= 2058 && code <= 2060)
        {
            return "CUAN";
            break;
        }
        if (code >= 2061 && code <= 2068)
        {
            return "CUI";
            break;
        }
        if (code >= 2069 && code <= 2071)
        {
            return "CUN";
            break;
        }
        if (code >= 2072 && code <= 2077)
        {
            return "CUO";
            break;
        }
        if (code >= 2078 && code <= 2083)
        {
            return "DA";
            break;
        }
        if (code >= 2084 && code <= 2094)
        {
            return "DAI";
            break;
        }
        if (code >= 2102 && code <= 2116)
        {
            return "DAN";
            break;
        }
        if (code >= 2117 && code <= 2121)
        {
            return "DANG";
            break;
        }
        if (code >= 2122 && code <= 2133)
        {
            return "DAO";
            break;
        }
        if (code >= 2134 && code <= 2136)
        {
            return "DE";
            break;
        }
        if (code >= 2137 && code <= 2143)
        {
            return "DENG";
            break;
        }
        if (code >= 2144 && code <= 2162)
        {
            return "DI";
            break;
        }
        if (code >= 2163 && code <= 2178)
        {
            return "DIAN";
            break;
        }
        if (code >= 2179 && code <= 2187)
        {
            return "DIAO";
            break;
        }
        if (code >= 2188 && code <= 2194)
        {
            return "DIE";
            break;
        }
        if (code >= 2201 && code <= 2209)
        {
            return "DING";
            break;
        }
        if (code == 2210)
        {
            return "DIU";
            break;
        }
        if (code >= 2211 && code <= 2220)
        {
            return "DONG";
            break;
        }
        if (code >= 2221 && code <= 2227)
        {
            return "DOU";
            break;
        }
        if (code >= 2228 && code <= 2242)
        {
            return "DU";
            break;
        }
        if (code >= 2243 && code <= 2248)
        {
            return "DUAN";
            break;
        }
        if (code >= 2249 && code <= 2252)
        {
            return "DUI";
            break;
        }
        if (code >= 2253 && code <= 2261)
        {
            return "DUN";
            break;
        }
        if (code >= 2262 && code <= 2273)
        {
            return "DUO";
            break;
        }
        if (code >= 2274 && code <= 2286)
        {
            return "E";
            break;
        }
        if (code == 2287)
        {
            return "EN";
            break;
        }
        if (code >= 2288 && code <= 2231)
        {
            return "ER";
            break;
        }
        if (code >= 2302 && code <= 2309)
        {
            return "FA";
            break;
        }
        if (code >= 2310 && code <= 2326)
        {
            return "FAN";
            break;
        }
        if (code >= 2327 && code <= 2337)
        {
            return "FANG";
            break;
        }
        if (code >= 2338 && code <= 2349)
        {
            return "FEI";
            break;
        }
        if (code >= 2350 && code <= 2364)
        {
            return "FEN";
            break;
        }
        if (code >= 2365 && code <= 2379)
        {
            return "FENG";
            break;
        }
        if (code == 2380)
        {
            return "FO";
            break;
        }
        if (code == 2381)
        {
            return "FOU";
            break;
        }
        if (code >= 2382 && code <= 2432)
        {
            return "FU";
            break;
        }
        if (code >= 2435 && code <= 2440)
        {
            return "GAI";
            break;
        }
        if (code >= 2441 && code <= 2451)
        {
            return "GAN";
            break;
        }
        if (code >= 2452 && code <= 2460)
        {
            return "GANG";
            break;
        }
        if (code >= 2461 && code <= 2470)
        {
            return "GAO";
            break;
        }
        if (code >= 2471 && code <= 2487)
        {
            return "GE";
            break;
        }
        if (code == 2488)
        {
            return "GEI";
            break;
        }
        if (code >= 2489 && code <= 2490)
        {
            return "GEN";
            break;
        }
        if (code >= 2491 && code <= 2503)
        {
            return "GENG";
            break;
        }
        if (code >= 2504 && code <= 2518)
        {
            return "GONG";
            break;
        }
        if (code >= 2519 && code <= 2527)
        {
            return "GOU";
            break;
        }
        if (code >= 2528 && code <= 2545)
        {
            return "GU";
            break;
        }
        if (code >= 2546 && code <= 2551)
        {
            return "GUA";
            break;
        }
        if (code >= 2552 && code <= 2554)
        {
            return "GUAI";
            break;
        }
        if (code >= 2555 && code <= 2565)
        {
            return "GUAN";
            break;
        }
        if (code >= 2566 && code <= 2568)
        {
            return "GUANG";
            break;
        }
        if (code >= 2569 && code <= 2584)
        {
            return "GUI";
            break;
        }
        if (code >= 2585 && code <= 2587)
        {
            return "GUN";
            break;
        }
        if (code >= 2588 && code <= 2593)
        {
            return "GUO";
            break;
        }
        if (code == 2594)
        {
            return "HA";
            break;
        }
        if (code >= 2601 && code <= 2607)
        {
            return "HAI";
            break;
        }
        if (code >= 2608 && code <= 2626)
        {
            return "HAN";
            break;
        }
        if (code >= 2627 && code <= 2629)
        {
            return "HANG";
            break;
        }
        if (code >= 2630 && code <= 2638)
        {
            return "HAO";
            break;
        }
        if (code >= 2639 && code <= 2656)
        {
            return "HE";
            break;
        }
        if (code >= 2657 && code <= 2658)
        {
            return "HEI";
            break;
        }
        if (code >= 2659 && code <= 2662)
        {
            return "HEN";
            break;
        }
        if (code >= 2663 && code <= 2667)
        {
            return "HENG";
            break;
        }
        if (code >= 2668 && code <= 2676)
        {
            return "HONG";
            break;
        }
        if (code >= 2677 && code <= 2683)
        {
            return "HOU";
            break;
        }
        if (code >= 2684 && code <= 2707)
        {
            return "HU";
            break;
        }
        if (code >= 2708 && code <= 2716)
        {
            return "HUA";
            break;
        }
        if (code >= 2717 && code <= 2721)
        {
            return "HUAI";
            break;
        }
        if (code >= 2722 && code <= 2735)
        {
            return "HUAN";
            break;
        }
        if (code >= 2736 && code <= 2749)
        {
            return "HUANG";
            break;
        }
        if (code >= 2750 && code <= 2770)
        {
            return "HUI";
            break;
        }
        if (code >= 2771 && code <= 2776)
        {
            return "HUN";
            break;
        }
        if (code >= 2777 && code <= 2786)
        {
            return "HUO";
            break;
        }
        if (code >= 2787 && code <= 2845)
        {
            return "JI";
            break;
        }
        if (code >= 2846 && code <= 2862)
        {
            return "JIA";
            break;
        }
        if (code >= 2863 && code <= 2908)
        {
            return "JIAN";
            break;
        }
        if (code >= 2909 && code <= 2921)
        {
            return "JIANG";
            break;
        }
        if (code >= 2922 && code <= 2949)
        {
            return "JIAO";
            break;
        }
        if (code >= 2950 && code <= 2976)
        {
            return "JIE";
            break;
        }
        if (code >= 2977 && code <= 3002)
        {
            return "JIN";
            break;
        }
        if (code >= 3003 && code <= 3027)
        {
            return "JING";
            break;
        }
        if (code >= 3028 && code <= 3029)
        {
            return "JIONG";
            break;
        }
        if (code >= 3030 && code <= 3046)
        {
            return "JIU";
            break;
        }
        if (code >= 3047 && code <= 3071)
        {
            return "JU";
            break;
        }
        if (code >= 3072 && code <= 3078)
        {
            return "JUAN";
            break;
        }
        if (code >= 3079 && code <= 3088)
        {
            return "JUE";
            break;
        }
        if (code >= 3089 && code <= 3105)
        {
            return "JUN";
            break;
        }
        if (code >= 3106 && code <= 3109)
        {
            return "KA";
            break;
        }
        if (code >= 3110 && code <= 3114)
        {
            return "KAI";
            break;
        }
        if (code >= 3115 && code <= 3120)
        {
            return "KAN";
            break;
        }
        if (code >= 3121 && code <= 3127)
        {
            return "KANG";
            break;
        }
        if (code >= 3128 && code <= 3131)
        {
            return "KAO";
            break;
        }
        if (code >= 3132 && code <= 3146)
        {
            return "KE";
            break;
        }
        if (code >= 3147 && code <= 3150)
        {
            return "KEN";
            break;
        }
        if (code >= 3151 && code <= 3152)
        {
            return "KENG";
            break;
        }
        if (code >= 3153 && code <= 3156)
        {
            return "KONG";
            break;
        }
        if (code >= 3157 && code <= 3160)
        {
            return "KOU";
            break;
        }
        if (code >= 3161 && code <= 3167)
        {
            return "KU";
            break;
        }
        if (code >= 3168 && code <= 3172)
        {
            return "KUA";
            break;
        }
        if (code >= 3173 && code <= 3176)
        {
            return "KUAI";
            break;
        }
        if (code >= 3177 && code <= 3178)
        {
            return "KUAN";
            break;
        }
        if (code >= 3179 && code <= 3186)
        {
            return "KUANG";
            break;
        }
        if (code >= 3187 && code <= 3203)
        {
            return "KUI";
            break;
        }
        if (code >= 3204 && code <= 3207)
        {
            return "KUN";
            break;
        }
        if (code >= 3208 && code <= 3211)
        {
            return "KUO";
            break;
        }
        if (code >= 3212 && code <= 3218)
        {
            return "LA";
            break;
        }
        if (code >= 3219 && code <= 3221)
        {
            return "LAI";
            break;
        }
        if (code >= 3222 && code <= 3236)
        {
            return "LAN";
            break;
        }
        if (code >= 3237 && code <= 3243)
        {
            return "LANG";
            break;
        }
        if (code >= 3244 && code <= 3252)
        {
            return "LAO";
            break;
        }
        if (code >= 3253 && code <= 3254)
        {
            return "LE";
            break;
        }
        if (code >= 3255 && code <= 3265)
        {
            return "LEI";
            break;
        }
        if (code >= 3266 && code <= 3268)
        {
            return "LENG";
            break;
        }
        if (code >= 3269 && code <= 3308)
        {
            return "LI";
        }
        if (code == 3309)
        {
            return "LIA";
            break;
        }
        if (code >= 3310 && code <= 3323)
        {
            return "LIAN";
            break;
        }
        if (code >= 3324 && code <= 3334)
        {
            return "LIANG";
            break;
        }
        if (code >= 3335 && code <= 3347)
        {
            return "LIAO";
            break;
        }
        if (code >= 3348 && code <= 3352)
        {
            return "LIE";
            break;
        }
        if (code >= 3353 && code <= 3363)
        {
            return "LIN";
            break;
        }
        if (code >= 3364 && code <= 3378)
        {
            return "LING";
            break;
        }
        if (code >= 3379 && code <= 3389)
        {
            return "LIU";
            break;
        }
        if (code >= 3390 && code <= 3404)
        {
            return "LONG";
            break;
        }
        if (code >= 3405 && code <= 3410)
        {
            return "LOU";
            break;
        }
        if (code >= 3411 && code <= 3444)
        {
            return "LU";
            break;
        }
        if (code >= 3445 && code <= 3450)
        {
            return "LUAN";
            break;
        }
        if (code >= 3451 && code <= 3452)
        {
            return "LUE";
            break;
        }
        if (code >= 3453 && code <= 3459)
        {
            return "LUN";
            break;
        }
        if (code >= 3460 && code <= 3471)
        {
            return "LUO";
            break;
        }
        if (code >= 3472 && code <= 3480)
        {
            return "MA";
            break;
        }
        if (code >= 3481 && code <= 3486)
        {
            return "MAI";
            break;
        }
        if (code >= 3487 && code <= 3501)
        {
            return "MAN";
            break;
        }
        if (code >= 3502 && code <= 3507)
        {
            return "MANG";
            break;
        }
        if (code >= 3508 && code <= 3519)
        {
            return "MAO";
            break;
        }
        if (code == 3520)
        {
            return "ME";
            break;
        }
        if (code >= 3521 && code <= 3536)
        {
            return "MEI";
            break;
        }
        if (code >= 3537 && code <= 3539)
        {
            return "MEN";
            break;
        }
        if (code >= 3540 && code <= 3547)
        {
            return "MENG";
            break;
        }
        if (code >= 3548 && code <= 3561)
        {
            return "MI";
        }
        if (code >= 3562 && code <= 3570)
        {
            return "MIAN";
            break;
        }
        if (code >= 3571 && code <= 3578)
        {
            return "MIAO";
            break;
        }
        if (code >= 3579 && code <= 3580)
        {
            return "MIE";
            break;
        }
        if (code >= 3581 && code <= 3586)
        {
            return "MIN";
            break;
        }
        if (code >= 3587 && code <= 3592)
        {
            return "MING";
            break;
        }
        if (code == 3593)
        {
            return "MIU";
            break;
        }
        if (code >= 3594 && code <= 3616)
        {
            return "MO";
            break;
        }
        if (code >= 3617 && code <= 3619)
        {
            return "MOU";
            break;
        }
        if (code >= 3620 && code <= 3634)
        {
            return "MU";
            break;
        }
        if (code >= 3635 && code <= 3641)
        {
            return "NA";
            break;
        }
        if (code >= 3642 && code <= 3646)
        {
            return "NAI";
            break;
        }
        if (code >= 3647 && code <= 3649)
        {
            return "NAN";
            break;
        }
        if (code == 3650)
        {
            return "NANG";
            break;
        }
        if (code >= 3651 && code <= 3655)
        {
            return "NAO";
            break;
        }
        if (code == 3656)
        {
            return "NE";
            break;
        }
        if (code >= 3657 && code <= 3658)
        {
            return "NEI";
            break;
        }
        if (code == 3659)
        {
            return "NEN";
            break;
        }
        if (code == 3660)
        {
            return "NENG";
            break;
        }
        if (code >= 3661 && code <= 3671)
        {
            return "NI";
            break;
        }
        if (code >= 3672 && code <= 3678)
        {
            return "NIAN";
            break;
        }
        if (code >= 3679 && code <= 3680)
        {
            return "NIANG";
            break;
        }
        if (code >= 3681 && code <= 3682)
        {
            return "NIAO";
            break;
        }
        if (code >= 3683 && code <= 3689)
        {
            return "NIE";
            break;
        }
        if (code == 3690)
        {
            return "NIN";
            break;
        }
        if (code >= 3691 && code <= 3702)
        {
            return "NING";
            break;
        }
        if (code >= 3703 && code <= 3706)
        {
            return "NIU";
            break;
        }
        if (code >= 3707 && code <= 3710)
        {
            return "NONG";
            break;
        }
        if (code >= 3711 && code <= 3714)
        {
            return "NU";
            break;
        }
        if (code == 3715)
        {
            return "NUAN";
            break;
        }
        if (code >= 3716 && code <= 3717)
        {
            return "NUE";
            break;
        }
        if (code >= 3718 && code <= 3721)
        {
            return "NUO";
            break;
        }
        if (code == 3722)
        {
            return "O";
            break;
        }
        if (code >= 3723 && code <= 3729)
        {
            return "OU";
            break;
        }
        if (code >= 3730 && code <= 3735)
        {
            return "PA";
            break;
        }
        if (code >= 3736 && code <= 3741)
        {
            return "PAI";
            break;
        }
        if (code >= 3742 && code <= 3749)
        {
            return "PAN";
            break;
        }
        if (code >= 3750 && code <= 3754)
        {
            return "PANG";
            break;
        }
        if (code >= 3755 && code <= 3761)
        {
            return "PAO";
            break;
        }
        if (code >= 3762 && code <= 3770)
        {
            return "PEI";
            break;
        }
        if (code >= 3771 && code <= 3772)
        {
            return "PEN";
            break;
        }
        if (code >= 3773 && code <= 3786)
        {
            return "PENG";
            break;
        }
        if (code >= 3787 && code <= 3809)
        {
            return "PI";
            break;
        }
        if (code >= 3810 && code <= 3813)
        {
            return "PIAN";
            break;
        }
        if (code >= 3814 && code <= 3817)
        {
            return "PIAO";
            break;
        }
        if (code >= 3818 && code <= 3819)
        {
            return "PIE";
            break;
        }
        if (code >= 3820 && code <= 3824)
        {
            return "PIN";
            break;
        }
        if (code >= 3825 && code <= 3833)
        {
            return "PING";
            break;
        }
        if (code >= 3834 && code <= 3841)
        {
            return "PO";
            break;
        }
        if (code == 3842)
        {
            return "POU";
            break;
        }
        if (code >= 3843 && code <= 3857)
        {
            return "PU";
            break;
        }
        if (code >= 3858 && code <= 3893)
        {
            return "QI";
            break;
        }
        if (code == 3894 || (code >= 3901 && code <= 3902))
        {
            return "QIA";
            break;
        }
        if (code >= 3903 && code <= 3924)
        {
            return "QIAN";
            break;
        }
        if (code >= 3925 && code <= 3932)
        {
            return "QIANG";
            break;
        }
        if (code >= 3933 && code <= 3947)
        {
            return "QIAO";
            break;
        }
        if (code >= 3948 && code <= 3952)
        {
            return "QIE";
            break;
        }
        if (code >= 3953 && code <= 3963)
        {
            return "QIN";
            break;
        }
        if (code >= 3964 && code <= 3976)
        {
            return "QING";
            break;
        }
        if (code >= 3977 && code <= 3978)
        {
            return "QIONG";
            break;
        }
        if (code >= 3979 && code <= 3986)
        {
            return "QIU";
            break;
        }
        if (code >= 3987 && code <= 4005)
        {
            return "QU";
            break;
        }
        if (code >= 4006 && code <= 4016)
        {
            return "QUAN";
            break;
        }
        if (code >= 4017 && code <= 4024)
        {
            return "QUE";
            break;
        }
        if (code >= 4025 && code <= 4026)
        {
            return "QUN";
            break;
        }
        if (code >= 4027 && code <= 4030)
        {
            return "RAN";
            break;
        }
        if (code >= 4031 && code <= 4035)
        {
            return "RANG";
        }
        if (code >= 4036 && code <= 4038)
        {
            return "RAO";
            break;
        }
        if (code >= 4039 && code <= 4040)
        {
            return "RE";
            break;
        }
        if (code >= 4041 && code <= 4050)
        {
            return "REN";
            break;
        }
        if (code >= 4051 && code <= 4052)
        {
            return "RENG";
            break;
        }
        if (code == 4053)
        {
            return "RI";
            break;
        }
        if (code >= 4054 && code <= 4063)
        {
            return "RONG";
            break;
        }
        if (code >= 4064 && code <= 4066)
        {
            return "ROU";
            break;
        }
        if (code >= 4067 && code <= 4076)
        {
            return "RU";
            break;
        }
        if (code >= 4077 && code <= 4078)
        {
            return "RUAN";
            break;
        }
        if (code >= 4079 && code <= 4081)
        {
            return "RUI";
            break;
        }
        if (code >= 4082 && code <= 4083)
        {
            return "RUN";
            break;
        }
        if (code >= 4084 && code <= 4085)
        {
            return "RUO";
            break;
        }
        if (code >= 4086 && code <= 4088)
        {
            return "SA";
            break;
        }
        if (code >= 4089 && code <= 4092)
        {
            return "SAI";
            break;
        }
        if (code >= 4093 && code <= 4094)
        {
            return "SAN";
            break;
        }
        if (code >= 4101 && code <= 4102)
        {
            return "SAN";
            break;
        }
        if (code >= 4103 && code <= 4105)
        {
            return "SANG";
            break;
        }
        if (code >= 4106 && code <= 4109)
        {
            return "SAO";
            break;
        }
        if (code >= 4110 && code <= 4112)
        {
            return "SE";
            break;
        }
        if (code == 4113)
        {
            return "SEN";
        }
        if (code == 4114)
        {
            return "SENG";
            break;
        }
        if (code >= 4115 && code <= 4123)
        {
            return "SHA";
            break;
        }
        if (code >= 4124 && code <= 4125)
        {
            return "SHAI";
            break;
        }
        if (code >= 4126 && code <= 4141)
        {
            return "SHAN";
            break;
        }
        if (code >= 4142 && code <= 4149)
        {
            return "SHANG";
            break;
        }
        if (code >= 4150 && code <= 4160)
        {
            return "SHAO";
            break;
        }
        if (code >= 4161 && code <= 4172)
        {
            return "SHE";
            break;
        }
        if (code >= 4173 && code <= 4188)
        {
            return "SHEN";
            break;
        }
        if (code >= 4189 && code <= 4205)
        {
            return "SHENG";
            break;
        }
        if (code >= 4206 && code <= 4252)
        {
            return "SHI";
            break;
        }
        if (code >= 4253 && code <= 4262)
        {
            return "SHOU";
            break;
        }
        if (code >= 4263 && code <= 4301)
        {
            return "SHU";
            break;
        }
        if (code >= 4302 && code <= 4303)
        {
            return "SHUA";
            break;
        }
        if (code >= 4304 && code <= 4307)
        {
            return "SHUAI";
            break;
        }
        if (code >= 4308 && code <= 4309)
        {
            return "SHUAN";
            break;
        }
        if (code >= 4310 && code <= 4312)
        {
            return "SHUANG";
            break;
        }
        if (code >= 4313 && code <= 4316)
        {
            return "SHUI";
            break;
        }
        if (code >= 4317 && code <= 4320)
        {
            return "SHUN";
            break;
        }
        if (code >= 4321 && code <= 4324)
        {
            return "SHUO";
            break;
        }
        if (code >= 4325 && code <= 4340)
        {
            return "SI";
            break;
        }
        if (code >= 4341 && code <= 4348)
        {
            return "SONG";
            break;
        }
        if (code >= 4349 && code <= 4352)
        {
            return "SOU";
            break;
        }
        if (code >= 4353 && code <= 4364)
        {
            return "SU";
            break;
        }
        if (code >= 4365 && code <= 4367)
        {
            return "SUAN";
            break;
        }
        if (code >= 4368 && code <= 4378)
        {
            return "SUI";
            break;
        }
        if (code >= 4379 && code <= 4381)
        {
            return "SUN";
            break;
        }
        if (code >= 4382 && code <= 4389)
        {
            return "SUO";
            break;
        }
        if (code >= 4390 && code <= 4404)
        {
            return "TA";
            break;
        }
        if (code >= 4405 && code <= 4413)
        {
            return "TAI";
            break;
        }
        if (code >= 4414 && code <= 4431)
        {
            return "TAN";
            break;
        }
        if (code >= 4432 && code <= 4444)
        {
            return "TANG";
            break;
        }
        if (code >= 4445 && code <= 4455)
        {
            return "TAO";
            break;
        }
        if (code == 4456)
        {
            return "TE";
            break;
        }
        if (code >= 4457 && code <= 4460)
        {
            return "TENG";
            break;
        }
        if (code >= 4461 && code <= 4475)
        {
            return "TI";
            break;
        }
        if (code >= 4476 && code <= 4483)
        {
            return "TIAN";
            break;
        }
        if (code >= 4484 && code <= 4488)
        {
            return "TIAO";
            break;
        }
        if (code >= 4489 && code <= 4491)
        {
            return "TIE";
            break;
        }
        if (code >= 4492 && code <= 4507)
        {
            return "TING";
            break;
        }
        if (code >= 4508 && code <= 4520)
        {
            return "TONG";
            break;
        }
        if (code >= 4521 && code <= 4524)
        {
            return "TOU";
            break;
        }
        if (code >= 4525 && code <= 4535)
        {
            return "TU";
            break;
        }
        if (code >= 4536 && code <= 4537)
        {
            return "TUAN";
            break;
        }
        if (code >= 4538 && code <= 4543)
        {
            return "TUI";
            break;
        }
        if (code >= 4544 && code <= 4546)
        {
            return "TUN";
            break;
        }
        if (code >= 4547 && code <= 4557)
        {
            return "TUO";
            break;
        }
        if (code >= 4558 && code <= 4564)
        {
            return "WA";
            break;
        }
        if (code >= 4565 && code <= 4566)
        {
            return "WAI";
            break;
        }
        if (code >= 4567 && code <= 4583)
        {
            return "WAN";
            break;
        }
        if (code >= 4584 && code <= 4593)
        {
            return "WANG";
            break;
        }
        if (code >= 4594 && code <= 4632)
        {
            return "WEI";
            break;
        }
        if (code >= 4633 && code <= 4642)
        {
            return "WEN";
            break;
        }
        if (code >= 4643 && code <= 4645)
        {
            return "WENG";
            break;
        }
        if (code >= 4646 && code <= 4654)
        {
            return "WO";
            break;
        }
        if (code >= 4655 && code <= 4683)
        {
            return "WU";
            break;
        }
        if (code >= 4684 && code <= 4724)
        {
            return "XI";
            break;
        }
        if (code >= 4725 && code <= 4737)
        {
            return "XIA";
            break;
        }
        if (code >= 4738 && code <= 4763)
        {
            return "XIAN";
            break;
        }
        if (code >= 4764 && code <= 4783)
        {
            return "XIANG";
            break;
        }
        if (code >= 4784 && code <= 4807)
        {
            return "XIAO";
            break;
        }
        if (code >= 4809 && code <= 4828)
        {
            return "XIE";
            break;
        }
        if (code >= 4829 && code <= 4838)
        {
            return "XIN";
            break;
        }
        if (code >= 4839 && code <= 4853)
        {
            return "XING";
            break;
        }
        if (code >= 4854 && code <= 4860)
        {
            return "XIONG";
            break;
        }
        if (code >= 4861 && code <= 4869)
        {
            return "XIU";
            break;
        }
        if (code >= 4870 && code <= 4888)
        {
            return "XU";
            break;
        }
        if (code >= 4889 && code <= 4904)
        {
            return "XUAN";
            break;
        }
        if (code >= 4905 && code <= 4910)
        {
            return "XUE";
            break;
        }
        if (code >= 4911 && code <= 4924)
        {
            return "XUN";
            break;
        }
        if (code >= 4925 && code <= 4940)
        {
            return "YA";
            break;
        }
        if (code >= 4941 && code <= 4973)
        {
            return "YAN";
            break;
        }
        if (code >= 4974 && code <= 4990)
        {
            return "YANG";
            break;
        }
        if (code >= 4991 && code <= 5011)
        {
            return "YAO";
            break;
        }
        if (code >= 5012 && code <= 5026)
        {
            return "YE";
            break;
        }
        if (code >= 5027 && code <= 5079)
        {
            return "YI";
            break;
        }
        if (code >= 5080 && code <= 5101)
        {
            return "YIN";
            break;
        }
        if (code >= 5102 && code <= 5119)
        {
            return "YING";
            break;
        }
        if (code == 5120)
        {
            return "YO";
            break;
        }
        if (code >= 5121 && code <= 5135)
        {
            return "YONG";
            break;
        }
        if (code >= 5136 && code <= 5155)
        {
            return "YOU";
            break;
        }
        if (code >= 5156 && code <= 5206)
        {
            return "YU";
            break;
        }
        if (code >= 5207 && code <= 5226)
        {
            return "YUAN";
            break;
        }
        if (code >= 5227 && code <= 5236)
        {
            return "YUE";
            break;
        }
        if (code >= 5237 && code <= 5248)
        {
            return "YUN";
            break;
        }
        if (code >= 5249 && code <= 5251)
        {
            return "ZA";
            break;
        }
        if (code >= 5252 && code <= 5258)
        {
            return "ZAI";
            break;
        }
        if (code >= 5259 && code <= 5262)
        {
            return "ZAN";
            break;
        }
        if (code >= 5263 && code <= 5265)
        {
            return "ZANG";
            break;
        }
        if (code >= 5266 && code <= 5279)
        {
            return "ZAO";
            break;
        }
        if (code >= 5280 && code <= 5283)
        {
            return "ZE";
            break;
        }
        if (code == 5284)
        {
            return "ZEI";
            break;
        }
        if (code == 5285)
        {
            return "ZEN";
            break;
        }
        if (code >= 5286 && code <= 5289)
        {
            return "ZENG";
            break;
        }
        if (code >= 5290 && code <= 5309)
        {
            return "ZHA";
            break;
        }
        if (code >= 5310 && code <= 5315)
        {
            return "ZHAI";
            break;
        }
        if (code >= 5316 && code <= 5332)
        {
            return "ZHAN";
            break;
        }
        if (code >= 5333 && code <= 5347)
        {
            return "ZHANG";
            break;
        }
        if (code >= 5348 && code <= 5357)
        {
            return "ZHAO";
            break;
        }
        if (code >= 5358 && code <= 5367)
        {
            return "ZHE";
            break;
        }
        if (code >= 5368 && code <= 5383)
        {
            return "ZHEN";
            break;
        }
        if (code >= 5384 && code <= 5404)
        {
            return "ZHENG";
            break;
        }
        if (code >= 5405 && code <= 5447)
        {
            return "ZHI";
            break;
        }
        if (code >= 5448 && code <= 5458)
        {
            return "ZHONG";
            break;
        }
        if (code >= 5459 && code <= 5472)
        {
            return "ZHOU";
            break;
        }
        if (code >= 5473 && code <= 5504)
        {
            return "ZHU";
            break;
        }
        if (code >= 5505 && code <= 5506)
        {
            return "ZHUA";
            break;
        }
        if (code == 5507)
        {
            return "ZHUAI";
            break;
        }
        if (code >= 5508 && code <= 5513)
        {
            return "ZHUAN";
            break;
        }
        if (code >= 5514 && code <= 5520)
        {
            return "ZHUANG";
            break;
        }
        if (code >= 5521 && code <= 5526)
        {
            return "ZHUI";
            break;
        }
        if (code >= 5527 && code <= 5528)
        {
            return "ZHUN";
            break;
        }
        if (code >= 5529 && code <= 5539)
        {
            return "ZHUO";
            break;
        }
        if (code >= 5540 && code <= 5554)
        {
            return "ZI";
            break;
        }
        if (code >= 5555 && code <= 5561)
        {
            return "ZONG";
            break;
        }
        if (code >= 5562 && code <= 5565)
        {
            return "ZOU";
            break;
        }
        if (code >= 5566 && code <= 5573)
        {
            return "ZU";
            break;
        }
        if (code >= 5574 && code <= 5575)
        {
            return "ZUAN";
            break;
        }
        if (code >= 5576 && code <= 5579)
        {
            return "ZUI";
            break;
        }
        if (code >= 5580 && code <= 5581)
        {
            return "ZUN";
            break;
        }
        if (code >= 5582 && code <= 5589)
        {
            return "ZUO";
            break;
        }
    }
    return "?";
}