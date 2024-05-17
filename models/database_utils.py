def filter_db_ordered(cursor, filter_criteria):
    """
    Filters the MongoDB cursor based on filter_criteria and orders the results.
    :param cursor: MongoDB cursor object.
    :param filter_criteria: Dictionary of filter criteria.
    :return: Filtered and ordered list of results.
    """
    filtered_data = [doc for doc in cursor if all(
        doc.get(k) <= v if isinstance(v, (int, float)) else doc.get(k) == v
        for k, v in filter_criteria.items()
    )]
    # You can add ordering logic if needed
    return filtered_data
