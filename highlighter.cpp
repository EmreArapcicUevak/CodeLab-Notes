#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) {

    HighlightingRule rule;

    // KEYWORDS RULES
    keywordFormat.setForeground(QColor(85,155,212));
    const QString keywordPatternsOne[] = {

        QStringLiteral("\\balignas\\b"), QStringLiteral("\\balignof\\b"), QStringLiteral("\\band\\b"),
        QStringLiteral("\\band_eq\\b"), QStringLiteral("\\basm\\b"), QStringLiteral("\\bauto\\b"),
        QStringLiteral("\\bbitand\b"), QStringLiteral("\\bbitor\\b"), QStringLiteral("\\bbool\\b"),
        QStringLiteral("\\bcase\\b"), QStringLiteral("\\bcatch\\b"),QStringLiteral("\\bchar\\b"),
        QStringLiteral("\\bchar\\b"), QStringLiteral("\\bchar16_t\\b"), QStringLiteral("\\bchar32_t\\b"),
        QStringLiteral("\\bclass\\b"), QStringLiteral("\\bcompl\\b"), QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bconstexpr\\b"), QStringLiteral("\\bconst_cast\\b"), QStringLiteral("\\bdecltype\\b"),
        QStringLiteral("\\bdefault\\b"), QStringLiteral("\\bdelete\\b"), QStringLiteral("\\bdo\\b"),
        QStringLiteral("\\bdouble\\b"), QStringLiteral("\\bdynamic_cast\\b"), QStringLiteral("\\benum\\b"),
        QStringLiteral("\\bexplicit\\b"), QStringLiteral("\\bexport\\b"), QStringLiteral("\\bextern\\b"),
        QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bfriend\\b"),
        QStringLiteral("\\bgoto\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"), QStringLiteral("\\bmutable\\b"), QStringLiteral("\\bnamespace\\b"),
        QStringLiteral("\\bnew\\b"), QStringLiteral("\\bnoexcept\\b"), QStringLiteral("\\bnot\\b"),
        QStringLiteral("\\bnot_eq\\b"), QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bor\\b"), QStringLiteral("\\bor_eq\\b"), QStringLiteral("\\bprivate\\b"),
        QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"), QStringLiteral("\\bregister\\b"),
        QStringLiteral("\\breinterpret_cast\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bsizeof\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstatic_assert\\b"),
        QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bstruct\\b"), QStringLiteral("\\btemplate\\b"),
        QStringLiteral("\\bthis\\b"), QStringLiteral("\\bthread_local\\b"), QStringLiteral("\\bthrow\\b"),
        QStringLiteral("\\btrue\\b"), QStringLiteral("\\btry\\b"), QStringLiteral("\\btypedef\\b"),
        QStringLiteral("\\btypeid\\b"), QStringLiteral("\\btypename\\b"), QStringLiteral("\\bunion\\b"),
        QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\busing\\b"), QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bwchar_t\\b"),
        QStringLiteral("\\bxor\\b"), QStringLiteral("\\bxor_eq\\b"), QStringLiteral("#\\bdefine\\b"),
        QStringLiteral("#\\binclude\\b")

    };

    for (const QString& pattern : keywordPatternsOne) {

        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

    }


    keywordFormat.setForeground(QColor(211,155,210));
    const QString keywordPatternsTwo[] = {
        QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\belse\\b"),
        QStringLiteral("\\bfor\\b"), QStringLiteral("\\bif\\b"), QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\bswitch\\b"), QStringLiteral("\\bwhile\\b")

    };

    for (const QString& pattern : keywordPatternsTwo) {

        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

    }


    keywordFormat.setForeground(QColor(255,92,92));
    const QString keywordPatternsThree[] = {
        QStringLiteral("\\bstring\\b"), QStringLiteral("\\bString\\b"), QStringLiteral("\\bstd"),
        QStringLiteral("cout"), QStringLiteral("cin"), QStringLiteral("endl")
    };

    for (const QString& pattern : keywordPatternsThree) {

        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

    }

    // SIGNS RULES
    signsFormat.setForeground(QColor(48,213,200));
    const QString signsPatterns[] = {
        QStringLiteral("\\+"), QStringLiteral("-"), QStringLiteral("\\*"),
        QStringLiteral("/"), QStringLiteral("<"), QStringLiteral(">"),
        QStringLiteral("\\["), QStringLiteral("\\]"), QStringLiteral("\\{"),
        QStringLiteral("\\}"), QStringLiteral("\\("), QStringLiteral("\\)"),
        QStringLiteral(";")

    };

    for (const QString& pattern : signsPatterns) {

        rule.pattern = QRegularExpression(pattern);
        rule.format = signsFormat;
        highlightingRules.append(rule);

    }

    // CLASS RULES
    classFormat.setForeground(QColor("white"));
    // without space
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\{)"));
    rule.format = classFormat;
    highlightingRules.append(rule);
    // with space
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+ +(?=\\{)"));
    rule.format = classFormat;
    highlightingRules.append(rule);


    // QUOTATION RULES
    quotationFormat.setForeground(QColor(103,149,83));
    // ""
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    // ''
    rule.pattern = QRegularExpression(QStringLiteral("\'.*\'"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
    // < >
    rule.pattern = QRegularExpression(QStringLiteral("\\<.*\\>"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // FUNCTION RULES
    functionFormat.setForeground(QColor(255,215,0));
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+ +(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // NUMBERS RULES
    numberFormat.setForeground(QColor(206,145,120));
    //integers
    rule.pattern = QRegularExpression(QStringLiteral(" +\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("=+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("<+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral(">+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\++\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("-+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\*+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("/+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("%+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    //floating numbers without f"
    rule.pattern = QRegularExpression(QStringLiteral(" +\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("=+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("<+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral(">+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\++\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("-+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\*+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("/+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("%+\\d+\\.+\\d+"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    //floating numbers with f"
    rule.pattern = QRegularExpression(QStringLiteral(" +\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("=+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("<+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral(">+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\++\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("-+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("\\*+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("/+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("%+\\d+\\.+\\d+f"));
    rule.format = numberFormat;
    highlightingRules.append(rule);


    // COMMENTS RULES
    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(QColor(208,2,27));
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(QColor(208,2,27));
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

}

void Highlighter::highlightBlock(const QString &text) {

    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {

        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

        while (matchIterator.hasNext()) {

            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);

        }

    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {

        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;

        if (endIndex == -1) {

            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;

        } else {

            commentLength = endIndex - startIndex + match.capturedLength();

        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);

    }

}
