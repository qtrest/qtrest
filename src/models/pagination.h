#ifndef PAGINATIONTEST_H
#define PAGINATIONTEST_H

#include <QObject>

class Pagination : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Pagination(QObject *parent = 0);

    //Specify pagination policy count
    Q_PROPERTY(PaginationPolicy policy READ policy WRITE setPolicy NOTIFY policyChanged)

    //----PageNumberPolicy----
    //Specify perPage count
    Q_PROPERTY(int perPage READ perPage WRITE setPerPage NOTIFY perPageChanged)
    //Read only incremental currentPage
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    //Specify current page header. Default is incremental X-Pagination-Current-Page, increment by call fetchMore (X-Pagination-Page-Count)
    Q_PROPERTY(QString currentPageHeader READ currentPageHeader WRITE setCurrentPageHeader NOTIFY currentPageHeaderChanged)
    //Read only max total records count from X-Pagination-Total-Count
    Q_PROPERTY(int totalCount READ totalCount WRITE setTotalCount NOTIFY totalCountChanged)
    //Max total records count. Default is X-Pagination-Total-Count
    Q_PROPERTY(QString totalCountHeader READ totalCountHeader WRITE setTotalCountHeader NOTIFY totalCountHeaderChanged)
    //Read only max page count from X-Pagination-Page-Count
    Q_PROPERTY(int pageCount READ pageCount WRITE setPageCount NOTIFY pageCountChanged)
    //Max page count. Default X-Pagination-Page-Count
    Q_PROPERTY(QString pageCountHeader READ pageCountHeader WRITE setPageCountHeader NOTIFY pageCountHeaderChanged)

    //----LimitOffsetPolicy----
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)

    //----CursorPolicy----
    Q_PROPERTY(int cursorQueryParam READ cursorQueryParam WRITE setCursorQueryParam NOTIFY cursorQueryParamChanged)
    Q_PROPERTY(int cursorValue READ cursorValue WRITE setCursorValue NOTIFY cursorValueChanged)

    //current model pagination policy
    //for example: http://www.django-rest-framework.org/api-guide/pagination/
    enum PaginationPolicy {
        None,
        PageNumber,
        LimitOffset,
        Cursor,
        Manual
    };

    Q_ENUMS(PaginationPolicy)

    int perPage() const;
    int currentPage() const;
    QString currentPageHeader() const;
    int totalCount() const;
    QString totalCountHeader() const;
    int pageCount() const;
    QString pageCountHeader() const;
    PaginationPolicy policy() const;

    int limit() const
    {
        return m_limit;
    }

    int offset() const
    {
        return m_offset;
    }

    int cursorQueryParam() const
    {
        return m_cursorQueryParam;
    }

    int cursorValue() const
    {
        return m_cursorValue;
    }

signals:
    void perPageChanged(int perPage);
    void currentPageChanged(int currentPage);
    void currentPageHeaderChanged(QString currentPageHeader);
    void totalCountChanged(int totalCount);
    void totalCountHeaderChanged(QString totalCountHeader);
    void pageCountChanged(int pageCount);
    void pageCountHeaderChanged(QString pageCountHeader);
    void policyChanged(PaginationPolicy policy);
    void limitChanged(int limit);
    void offsetChanged(int offset);
    void cursorQueryParamChanged(int cursorQueryParam);
    void cursorValueChanged(int cursorValue);

public slots:
    void setPerPage(int perPage);
    void setCurrentPage(int currentPage);
    void setCurrentPageHeader(QString currentPageHeader);
    void setTotalCount(int totalCount);
    void setTotalCountHeader(QString totalCountHeader);
    void setPageCount(int pageCount);
    void setPageCountHeader(QString pageCountHeader);

    void setPolicy(PaginationPolicy policy);

    void setLimit(int limit)
    {
        if (m_limit == limit)
            return;

        m_limit = limit;
        emit limitChanged(limit);
    }

    void setOffset(int offset)
    {
        if (m_offset == offset)
            return;

        m_offset = offset;
        emit offsetChanged(offset);
    }

    void setCursorQueryParam(int cursorQueryParam)
    {
        if (m_cursorQueryParam == cursorQueryParam)
            return;

        m_cursorQueryParam = cursorQueryParam;
        emit cursorQueryParamChanged(cursorQueryParam);
    }

    void setCursorValue(int cursorValue)
    {
        if (m_cursorValue == cursorValue)
            return;

        m_cursorValue = cursorValue;
        emit cursorValueChanged(cursorValue);
    }

private:
    int m_perPage;
    int m_currentPage = 0;
    QString m_currentPageHeader;
    int m_totalCount;
    QString m_totalCountHeader;
    int m_pageCount;
    QString m_pageCountHeader;
    PaginationPolicy m_policy;
    int m_limit;
    int m_offset;
    int m_cursorQueryParam;
    int m_cursorValue;
};

#endif // PAGINATIONTEST_H
