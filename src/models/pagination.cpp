#include "pagination.h"
#include <QDebug>

Pagination::Pagination(QObject *parent) : QObject(parent),
    m_perPage(20), m_currentPage(0), m_totalCount(0), m_pageCount(0),
    m_currentPageHeader("X-Pagination-Current-Page"), m_totalCountHeader("X-Pagination-Total-Count"), m_pageCountHeader("X-Pagination-Page-Count")
{
    qDebug() << "Pagination::Pagination" << m_perPage << m_currentPage;
}

int Pagination::perPage() const
{
    return m_perPage;
}

int Pagination::currentPage() const
{
    return m_currentPage;
}

QString Pagination::currentPageHeader() const
{
    return m_currentPageHeader;
}

int Pagination::totalCount() const
{
    return m_totalCount;
}

QString Pagination::totalCountHeader() const
{
    return m_totalCountHeader;
}

int Pagination::pageCount() const
{
    return m_pageCount;
}

QString Pagination::pageCountHeader() const
{
    return m_pageCountHeader;
}

Pagination::PaginationPolicy Pagination::policy() const
{
    return m_policy;
}

void Pagination::setPerPage(int perPage)
{
    qDebug() << "per page set" << perPage;
    if (m_perPage == perPage)
        return;

    m_perPage = perPage;
    emit perPageChanged(perPage);
}

void Pagination::setCurrentPage(int currentPage)
{
    if (m_currentPage == currentPage)
        return;

    m_currentPage = currentPage;
    emit currentPageChanged(currentPage);
}

void Pagination::setCurrentPageHeader(QString currentPageHeader)
{
    if (m_currentPageHeader == currentPageHeader)
        return;

    m_currentPageHeader = currentPageHeader;
    emit currentPageHeaderChanged(currentPageHeader);
}

void Pagination::setTotalCount(int totalCount)
{
    if (m_totalCount == totalCount)
        return;

    m_totalCount = totalCount;
    emit totalCountChanged(totalCount);
}

void Pagination::setTotalCountHeader(QString totalCountHeader)
{
    if (m_totalCountHeader == totalCountHeader)
        return;

    m_totalCountHeader = totalCountHeader;
    emit totalCountHeaderChanged(totalCountHeader);
}

void Pagination::setPageCount(int pageCount)
{
    if (m_pageCount == pageCount)
        return;

    m_pageCount = pageCount;
    emit pageCountChanged(pageCount);
}

void Pagination::setPageCountHeader(QString pageCountHeader)
{
    if (m_pageCountHeader == pageCountHeader)
        return;

    m_pageCountHeader = pageCountHeader;
    emit pageCountHeaderChanged(pageCountHeader);
}

void Pagination::setPolicy(Pagination::PaginationPolicy policy)
{
    if (m_policy == policy)
        return;

    m_policy = policy;
    emit policyChanged(policy);
}
